#include "spion.hh"


#include "common/net/listener.hh"
#include "common/net/poller.hh"
#include "common/net/socket.hh"

#include "common/protocol/types.hh"
#include "common/protocol/packet.hh"


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

	message(int);
	message(double);
	message(unsigned int);
	message(char const *);
	message(void const *, std::size_t size);

	common::protocol::data_type type;
	data_t data;
	std::vector<char> str_or_blob;
};

message::message(int value)
:type(common::protocol::data_type::_Int), data(), str_or_blob()
{
	data.i = value;
}

message::message(double value)
:type(common::protocol::data_type::_Double), data(), str_or_blob()
{
	data.d = value;
}

message::message(unsigned int value)
:type(common::protocol::data_type::_UInt), data(), str_or_blob()
{
	data.ui = value;
}

message::message(char const * value)
:type(common::protocol::data_type::_String), data(), str_or_blob()
{
	str_or_blob.reserve(std::strlen(value));
	for (std::size_t i = 0; value[i]; ++i)
		str_or_blob.push_back(value[i]);
}

message::message(void const * blob, std::size_t size)
:type(common::protocol::data_type::_Blob), data(), str_or_blob()
{
	str_or_blob.reserve(size);
	for (std::size_t i = 0; i < size; ++i)
		str_or_blob.push_back(((char const *)(blob))[i]);
}



////////////////////////////////////////////////////////////////////////////////
//                                SpionServer                                 //
////////////////////////////////////////////////////////////////////////////////



class SpionServer
{
public:
	SpionServer()  = default;
	~SpionServer() = default;

	bool init(unsigned short port_number);
	void clean();

	void send(char const *, message &&);

private:
	SpionServer(SpionServer const &)             = delete;
	SpionServer(SpionServer &&)                  = delete;
	SpionServer & operator=(SpionServer const &) = delete;
	SpionServer & operator=(SpionServer &&)      = delete;

	common::protocol::id_t find_id(char const *);
	void send(common::protocol::packet const &);

	common::net::listener          _listener;
	std::list<common::net::socket> _clients;
	common::net::poller            _poller;

	dico                           _dico;

	std::mutex                     _mutex_listener;
	std::mutex                     _mutex_clients;
	std::thread                    _th;

	static void thread_fct(SpionServer *);
};


bool SpionServer::init(unsigned short port_number)
{
	{
		std::lock_guard<std::mutex> lck(_mutex_listener);

		if (_listener.init(port_number) == false)
			return (false);

		_poller.add(_listener);
	}

	_th = std::thread(SpionServer::thread_fct, this);
	_th.detach();

	return (true);
}

void SpionServer::clean()
{
	{
		std::lock_guard<std::mutex> lck(_mutex_clients);
		for (auto it = _clients.begin(); it != _clients.end(); ++it) {
			// _poller.remove(*it);
			it->close();
		}		
	}

	{
		std::lock_guard<std::mutex> lck(_mutex_listener);
		_poller.remove(_listener);
		_listener.close();
	}
}

void SpionServer::send(char const * str_id, message && msg)
{
	common::protocol::packet_header_data header;
	common::protocol::packet p;

	header.id = find_id(str_id);
	header.type = msg.type;
	if (msg.type == common::protocol::data_type::_String ||
	    msg.type == common::protocol::data_type::_Blob)
	{
		header.size = msg.str_or_blob.size();
		p = common::protocol::make_packet(header, msg.str_or_blob.data());
	}
	else
	{
		if (msg.type == common::protocol::data_type::_Double) {
			common::protocol::double_data tmp(msg.data.d);
			header.size = sizeof(tmp);
			p = common::protocol::make_packet(header, &tmp);
		}
		else {
			header.size = sizeof(std::int32_t);
			p = common::protocol::make_packet(header, &(msg.data.i));
		}

	}

	send(p);
}

common::protocol::id_t SpionServer::find_id(char const * str_id)
{
	auto found = _dico.find(str_id);

	if (found.second == false) {
		common::protocol::packet_header_data dico_header;

		dico_header.id = found.first;
		dico_header.type = common::protocol::data_type::_Dico;
		dico_header.size = std::strlen(str_id);

		common::protocol::packet dico_packet = common::protocol::make_packet(dico_header, str_id);
		send(dico_packet);
	}

	return (found.first);
}

void SpionServer::send(common::protocol::packet const & p)
{
	std::lock_guard<std::mutex> lck(_mutex_clients);

	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		it->send(p.data(), p.size());
}

void SpionServer::thread_fct(SpionServer * self)
{
	for (;;) {

		{
			std::lock_guard<std::mutex> lck(self->_mutex_listener);
			if (self->_listener.is_ok() == false)
				return ;			
		}

		auto poll_list = self->_poller.poll(std::chrono::milliseconds(100));

		for (auto it = poll_list.begin(); it != poll_list.end(); ++it)
		{
			{
				std::lock_guard<std::mutex> lck_l(self->_mutex_listener);
				if (self->_listener == *it) {

					std::lock_guard<std::mutex> lck_c(self->_mutex_clients);

					self->_clients.emplace_back(self->_listener.accept());
					// self->_poller.add(_client.back());
				}

			}

		}

	} // end-thread-loop

}



SpionServer & bucket_for_server()
{
	static SpionServer server;
	return (server);
}



/*
                                        _
                            _________  (_)___  ____
                           / ___/ __ \/ / __ \/ __ \
                          (__  ) /_/ / / /_/ / / / /
                         /____/ .___/_/\____/_/ /_/
                             /_/
*/



bool spion::init(unsigned short port_number)
{
	return (bucket_for_server().init(port_number));
}

void spion::clean()
{
	bucket_for_server().clean();
}



void spion::send_ro(char const * str_id, int value)
{
	bucket_for_server().send(str_id, message(value));
}

void spion::send_ro(char const * str_id, double value)
{
	bucket_for_server().send(str_id, message(value));
}

void spion::send_ro(char const * str_id, unsigned int value)
{
	bucket_for_server().send(str_id, message(value));
}

void spion::send_ro(char const * str_id, char const * value)
{
	bucket_for_server().send(str_id, message(value));
}

void spion::send_ro(char const * str_id, void const * blob, std::size_t size)
{
	bucket_for_server().send(str_id, message(blob, size));
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
