#ifndef SPION_SERVER_HH_INCLUDED
#define SPION_SERVER_HH_INCLUDED



#include "common/net/listener.hh"
#include "common/net/poller.hh"
#include "common/net/socket.hh"

#include "common/protocol/string.hh"

#include "Client.hh"

#include <list>
#include <string>
#include <thread>
#include <mutex>



namespace spion
{



class Server
{
public:
	Server()  = default;
	~Server() = default;

	bool init(unsigned short port_number);
	void clean();

	void send(const char * id_str, int);
	void send(const char * id_str, unsigned int);
	void send(const char * id_str, double);
	void send(const char * id_str, char const *);

private:
	Server(Server const &)             = delete;
	Server(Server &&)                  = delete;
	Server & operator=(Server const &) = delete;
	Server & operator=(Server &&)      = delete;

	common::net::listener  _listener;
	common::net::poller    _poller;

	std::list<Client> _clients;

	std::thread _th;
	std::mutex  _mtx;

	static void thread_fct(Server *);
	bool _thread_fct();

	void on_new_client(common::net::socket &&);
	void on_client_wake(common::net::socket_handler_t);

	template <class T>
	void _send(const char * id_str, T value);
};


template <class T>
void Server::_send(const char * id_str, T value)
{
	auto payload = common::protocol::string::make(id_str, value);

	{
		std::lock_guard<std::mutex> lck(_mtx);
		for (auto it = _clients.begin(); it != _clients.end(); ++it)
			if (it->is_listening_to(id_str))
				it->send(payload);
	}
}


} // spion



#endif  // SPION_SERVER_HH_INCLUDED
