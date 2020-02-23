#include "spion.hh"


#include "common/net/listener.hh"
#include "common/net/poller.hh"
#include "common/net/socket.hh"

#include "common/protocol/string.hh"
#include "common/protocol/types.hh"


#include <vector>
#include <list>
#include <map>
#include <string>

#include <algorithm>

#include <thread>
#include <mutex>

#include <cstring>


#include "logg.hh"


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

	void send(std::string const &);
	void send(const char * id_str, int);
	void send(const char * id_str, unsigned int);
	void send(const char * id_str, double);
	void send(const char * id_str, char const *);

private:
	SpionServer(SpionServer const &)             = delete;
	SpionServer(SpionServer &&)                  = delete;
	SpionServer & operator=(SpionServer const &) = delete;
	SpionServer & operator=(SpionServer &&)      = delete;

	common::net::listener  _listener;
	common::net::poller    _poller;

	std::list<common::net::socket> _clients;

	std::thread _th;
	std::mutex  _mtx;

	static void thread_fct(SpionServer *);

	void on_new_client(common::net::socket &&);
	void on_client_wake(common::net::socket_handler_t);
};


bool SpionServer::init(unsigned short port_number)
{
	if (_listener.init(port_number) == false)
		return (false);

	_poller.add(_listener);

	_th = std::thread(SpionServer::thread_fct, this);

	return (true);
}

void SpionServer::clean()
{
	std::lock_guard<std::mutex> lck(_mtx);

	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		it->close();

	_listener.close();

	_th.join();
}



void SpionServer::send(std::string const & str)
{
	std::lock_guard<std::mutex> lck(_mtx);

	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		common::protocol::string::send(*it, str);
}


void SpionServer::send(const char * id_str, int value)
{
	std::lock_guard<std::mutex> lck(_mtx);

	auto payload = common::protocol::string::make(id_str, value);

	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		common::protocol::string::send(*it, payload);	
}

void SpionServer::send(const char * id_str, unsigned int value)
{
	std::lock_guard<std::mutex> lck(_mtx);

	auto payload = common::protocol::string::make(id_str, value);

	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		common::protocol::string::send(*it, payload);	
}

void SpionServer::send(const char * id_str, double value)
{
	std::lock_guard<std::mutex> lck(_mtx);

	auto payload = common::protocol::string::make(id_str, value);

	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		common::protocol::string::send(*it, payload);	
}

void SpionServer::send(const char * id_str, char const * value)
{
	std::lock_guard<std::mutex> lck(_mtx);

	auto payload = common::protocol::string::make(id_str, value);

	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		common::protocol::string::send(*it, payload);	
}





void SpionServer::on_new_client(common::net::socket && s)
{
	_clients.emplace_back(std::move(s));
	_poller.add(_clients.back());

	COUT_INFO << "Client connected";
}

void SpionServer::on_client_wake(common::net::socket_handler_t s)
{
	auto found = std::find(_clients.begin(), _clients.end(), s);
	if (found == _clients.end())
		return ;

	if (found->recv(nullptr, 0) == 0) {
		_poller.remove(*found);
		_clients.erase(found);

		COUT_INFO << "Client disconnected";
	}
}



void SpionServer::thread_fct(SpionServer * self)
{

	for (;;)
	{
		auto tmp = self->_poller.poll(std::chrono::milliseconds(100));

		{
			std::lock_guard<std::mutex> lck(self->_mtx);

			if (self->_listener.is_ok() == false)
				return ;

			for (auto it = tmp.begin(); it != tmp.end(); ++it)
				if (self->_listener == *it)
					self->on_new_client(self->_listener.accept());
				else
					self->on_client_wake(*it);

		} // mutex

	} // threadl loop

}





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



void spion::send_ro(char const * str)
{
	_static_spion_server().send(str);
}

void spion::send_ro(std::string const & str)
{
	_static_spion_server().send(str);
}



void spion::send_ro(const char * id_str, int value)
{
	_static_spion_server().send(id_str, value);	
}

void spion::send_ro(const char * id_str, unsigned int value)
{
	_static_spion_server().send(id_str, value);	
}

void spion::send_ro(const char * id_str, double value)
{
	_static_spion_server().send(id_str, value);	
}

void spion::send_ro(const char * id_str, char const * value)
{
	_static_spion_server().send(id_str, value);	
}

