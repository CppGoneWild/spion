#include "Server.hh"

#include "common/protocol/string.hh"
#include "common/protocol/types.hh"

#include <algorithm>
#include <cstring>

#include "logg.hh"



bool spion::Server::init(unsigned short port_number)
{
	if (_listener.init(port_number) == false)
		return (false);

	_poller.add(_listener);

	_th = std::thread(spion::Server::thread_fct, this);

	return (true);
}

void spion::Server::clean()
{
	std::lock_guard<std::mutex> lck(_mtx);

	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		it->close();

	_listener.close();

	_th.join();
}



void spion::Server::send(std::string const & str)
{
	std::lock_guard<std::mutex> lck(_mtx);

	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		common::protocol::string::send(*it, str);
}


void spion::Server::send(const char * id_str, int value)
{
	std::lock_guard<std::mutex> lck(_mtx);

	auto payload = common::protocol::string::make(id_str, value);

	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		common::protocol::string::send(*it, payload);	
}

void spion::Server::send(const char * id_str, unsigned int value)
{
	std::lock_guard<std::mutex> lck(_mtx);

	auto payload = common::protocol::string::make(id_str, value);

	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		common::protocol::string::send(*it, payload);	
}

void spion::Server::send(const char * id_str, double value)
{
	std::lock_guard<std::mutex> lck(_mtx);

	auto payload = common::protocol::string::make(id_str, value);

	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		common::protocol::string::send(*it, payload);	
}

void spion::Server::send(const char * id_str, char const * value)
{
	std::lock_guard<std::mutex> lck(_mtx);

	auto payload = common::protocol::string::make(id_str, value);

	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		common::protocol::string::send(*it, payload);	
}





void spion::Server::on_new_client(common::net::socket && s)
{
	_clients.emplace_back(std::move(s));
	_poller.add(_clients.back());

	COUT_INFO << "Client connected";
}

void spion::Server::on_client_wake(common::net::socket_handler_t s)
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



void spion::Server::thread_fct(spion::Server * self)
{
	while (self->_thread_fct())
	{}
}

bool spion::Server::_thread_fct()
{
	auto tmp = _poller.poll(std::chrono::milliseconds(100));

	{
		std::lock_guard<std::mutex> lck(_mtx);

		if (_listener.is_ok() == false)
			return (false);

		for (auto it = tmp.begin(); it != tmp.end(); ++it)
			if (_listener == *it)
				on_new_client(_listener.accept());
			else
				on_client_wake(*it);

	} // mutex

	return (true);
}
