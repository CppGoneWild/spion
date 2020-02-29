#include "Server.hh"

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

void spion::Server::send(const char * id_str, int value)
{
	auto payload = common::protocol::string::make(id_str, value);

	_send(id_str, payload);
}

void spion::Server::send(const char * id_str, unsigned int value)
{
	auto payload = common::protocol::string::make(id_str, value);

	_send(id_str, payload);
}

void spion::Server::send(const char * id_str, double value)
{
	auto payload = common::protocol::string::make(id_str, value);

	_send(id_str, payload);
}

void spion::Server::send(const char * id_str, char const * value)
{
	auto payload = common::protocol::string::make(id_str, value);

	_send(id_str, payload);
}



void spion::Server::thread_fct(spion::Server * self)
{
	while (self->_thread_fct()) ;
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



void spion::Server::on_new_client(common::net::socket && s)
{
	_clients.emplace_back(std::move(s));
	_poller.add(_clients.back().socket());

	_clients.back().send("l ID_NAME to listen to variable\r\n");

	COUT_INFO << "Client connected";
}

void spion::Server::on_client_wake(common::net::socket_handler_t s)
{
	auto found = std::find(_clients.begin(), _clients.end(), s);
	if (found == _clients.end())
		return ;

	common::protocol::string::payload payload;

	if (found->recv(payload) == Client::recv_event::disconnection) {
		_poller.remove(found->socket());
		_clients.erase(found);

		COUT_INFO << "Client disconnected";
	}
	else if (found->execute_remote_cmd(payload) == false)
		COUT_INFO << payload.c_str();
}

void spion::Server::_send(char const * id_str, common::protocol::string::payload const & payload)
{
	std::lock_guard<std::mutex> lck(_mtx);
	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		if (it->is_listening_to(id_str))
			it->send(payload);
}
