#include "spion.hh"


#include "common/net/listener.hh"
#include "common/net/poller.hh"
#include "common/net/socket.hh"

#include "common/protocol/packet.hh"
#include "common/protocol/types.hh"


#include <vector>
#include <list>
#include <map>

#include <thread>
#include <mutex>

#include <cstring>



////////////////////////////////////////////////////////////////////////////////
//                              id_generator<T>                               //
////////////////////////////////////////////////////////////////////////////////



template <class T>
class id_generator
{
public:
	id_generator()  = default;
	~id_generator() = default;

	T next() { return (++_last); }

private:
	id_generator(id_generator<T> const &)                = delete;
	id_generator(id_generator<T> &&)                     = delete;
	id_generator<T> & operator=(id_generator<T> const &) = delete;
	id_generator<T> & operator=(id_generator<T> &&)      = delete;

	T _last = (T)(0);
};



////////////////////////////////////////////////////////////////////////////////
//                                    dico                                    //
////////////////////////////////////////////////////////////////////////////////



class dico
{
public:
	using id_t = common::protocol::id_t;

	dico()  = default;
	~dico() = default;

	std::pair<id_t, bool> find(char const * str);

private:
	dico(dico const &)             = delete;
	dico(dico &&)                  = delete;
	dico & operator=(dico const &) = delete;
	dico & operator=(dico &&)      = delete;

	id_generator<id_t> _id_gen;
	std::map<std::string, id_t> _dico;
};


std::pair<dico::id_t, bool> dico::find(char const * str)
{
	auto found = _dico.find(str);
	if (found == _dico.end()) {
		id_t tmp = _id_gen.next();
		_dico.insert(std::pair(str, tmp));
		return (std::pair<id_t, bool>(tmp, false));
	}
	return (std::pair<id_t, bool>(found->second, true));
}



////////////////////////////////////////////////////////////////////////////////
//                                  message                                   //
////////////////////////////////////////////////////////////////////////////////



struct message
{
	union data_t
	{
		std::int32_t i;
		std::uint32_t ui;
		double d;
	};

	message(char const * id_str, int);
	message(char const * id_str, double);
	message(char const * id_str, unsigned int);
	message(char const * id_str, char const *);
	message(char const * id_str, void const *, std::size_t size);

	common::protocol::data_type type;
	std::string id_str;
	data_t data;
	std::vector<char> str_or_blob;
};



////////////////////////////////////////////////////////////////////////////////
//                                SpionServer                                 //
////////////////////////////////////////////////////////////////////////////////

class SpionClient
{
public:
	enum Protocol { String = 0, Packet = 1 };

	SpionClient()                           = default;
	SpionClient(SpionClient &&)             = default;
	~SpionClient()                          = default;
	SpionClient & operator=(SpionClient &&) = default;

	explicit SpionClient(common::net::socket &&);

	bool operator==(SpionClient const &) const;
	bool operator!=(SpionClient const &) const;

	bool send(message const &)
	message on_recv();

private:	
	SpionClient(SpionClient const &) = default;
	SpionClient & operator=(SpionClient const &) = default;

	common::net::socket _socket;
	Protocol _protocol = Protocol::String;
};



class SpionServer
{
public:
	SpionServer()  = default;
	~SpionServer() = default;

	bool init(unsigned short port_number);
	void clean();

	void send(message &&);

private:
	SpionServer(SpionServer const &)             = delete;
	SpionServer(SpionServer &&)                  = delete;
	SpionServer & operator=(SpionServer const &) = delete;
	SpionServer & operator=(SpionServer &&)      = delete;

	common::net::listener  _listener;
	std::list<SpionCLient> _clients;
	common::net::poller    _poller;

	dico                   _dico;

	std::thread            _th;

	static void thread_fct(SpionServer *);
};


bool SpionServer::init(unsigned short port_number)
{
	_th = std::thread(SpionServer::thread_fct, this);
	_th.detach();

	return (true);
}

void SpionServer::clean()
{}

void SpionServer::send(message && msg)
{
	message _msg(msg);

	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		it->send(_msg);
}

void SpionServer::thread_fct(SpionServer * self)
{}





/*
                                        _
                            _________  (_)___  ____
                           / ___/ __ \/ / __ \/ __ \
                          (__  ) /_/ / / /_/ / / / /
                         /____/ .___/_/\____/_/ /_/
                             /_/
*/




static SpionServer & _static_spion_server()
{
	static SpionServer server;
	return (server);
}



bool spion::init(unsigned short port_number)
{
	return (_static_spion_server().init(port_number));
}

void spion::clean()
{
	_static_spion_server().clean();
}



void spion::send_ro(char const * str_id, int value)
{
	_static_spion_server().send(message(str_id, value));
}

void spion::send_ro(char const * str_id, double value)
{
	_static_spion_server().send(message(str_id, value));
}

void spion::send_ro(char const * str_id, unsigned int value)
{
	_static_spion_server().send(message(str_id, value));
}

void spion::send_ro(char const * str_id, char const * value)
{
	_static_spion_server().send(message(str_id, value));
}

void spion::send_ro(char const * str_id, void const * blob, std::size_t size)
{
	_static_spion_server().send(message(str_id, blob, size));
}



void spion::send_rw(char const *, int &)
{}

void spion::send_rw(char const *, double &)
{}

void spion::send_rw(char const *, unsigned int &)
{}

void spion::send_rw(char const *, char *)
{}

void spion::send_rw(char const *, void *, std::size_t)
{}
