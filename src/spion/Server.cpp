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



void spion::Server::send_ro(const char * id_str, int value)
{
	auto payload = common::protocol::string::make(id_str, value);

	{
		std::lock_guard<std::mutex> lck(_mtx);
		auto found = _dico.find(id_str);
		if (found == _dico.end())
			_dico.insert(std::pair(std::string(id_str), bucket(bucket::type_t::data)));

		_send(id_str, payload);
	}
}

void spion::Server::send_ro(const char * id_str, unsigned int value)
{
	auto payload = common::protocol::string::make(id_str, value);

	{
		std::lock_guard<std::mutex> lck(_mtx);
		auto found = _dico.find(id_str);
		if (found == _dico.end())
			_dico.insert(std::pair(std::string(id_str), bucket(bucket::type_t::data)));

		_send(id_str, payload);
	}
}

void spion::Server::send_ro(const char * id_str, double value)
{
	auto payload = common::protocol::string::make(id_str, value);

	{
		std::lock_guard<std::mutex> lck(_mtx);
		auto found = _dico.find(id_str);
		if (found == _dico.end())
			_dico.insert(std::pair(std::string(id_str), bucket(bucket::type_t::data)));

		_send(id_str, payload);
	}
}

void spion::Server::send_ro(const char * id_str, char const * value)
{
	auto payload = common::protocol::string::make(id_str, value);

	{
		std::lock_guard<std::mutex> lck(_mtx);
		auto found = _dico.find(id_str);
		if (found == _dico.end())
			_dico.insert(std::pair(std::string(id_str), bucket(bucket::type_t::data)));

		_send(id_str, payload);
	}
}



void spion::Server::send_rw(const char * id_str, int & value)
{
	std::lock_guard<std::mutex> lck(_mtx);

	had_to_write(id_str, value);
	auto payload = common::protocol::string::make(id_str, value);
	_send(id_str, payload);
}

void spion::Server::send_rw(const char * id_str, unsigned int & value)
{
	std::lock_guard<std::mutex> lck(_mtx);

	had_to_write(id_str, value);
	auto payload = common::protocol::string::make(id_str, value);
	_send(id_str, payload);
}

void spion::Server::send_rw(const char * id_str, double & value)
{
	std::lock_guard<std::mutex> lck(_mtx);

	had_to_write(id_str, value);
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
	_clients.emplace_back(std::move(s), _dico);
	_poller.add(_clients.back().socket());


	COUT_INFO << "Client connected";
}

void spion::Server::on_client_wake(common::net::socket_handler_t s)
{
	auto found = std::find(_clients.begin(), _clients.end(), s);
	if (found == _clients.end())
		return ;

	if (found->on_recv() == Client::recv_event::disconnection) {
		_poller.remove(found->socket());
		_clients.erase(found);

		COUT_INFO << "Client disconnected";
	}
}



void spion::Server::_send(char const * id_str, common::protocol::payload const & payload)
{
	for (auto it = _clients.begin(); it != _clients.end(); ++it)
		if (it->is_listening_to(id_str))
			it->send(payload);
}





bool spion::Server::had_to_write(const char * id_str, int & value)
{
	auto found = _dico.find(id_str);
	if (found == _dico.end()) {
		_dico.insert(std::pair(std::string(id_str), bucket(bucket::type_t::data)));
		return (false);
	}

	if (found->second.has_data && found->second.type == bucket::type_t::data) {

		try {
			value = std::stoi(found->second.data);
		}
		catch (...) {
			COUT_ERROR << "'" << found->second.data << "' not a valid integer.";
		}

		found->second.has_data = false;
		_send(id_str, common::protocol::string::make(id_str, value));
		return (true);
	}
	return (false);
}

bool spion::Server::had_to_write(const char * id_str, unsigned int & value)
{
	auto found = _dico.find(id_str);
	if (found == _dico.end()) {
		_dico.insert(std::pair(std::string(id_str), bucket(bucket::type_t::data)));
		return (false);
	}

	if (found->second.has_data && found->second.type == bucket::type_t::data) {

		try {
			value = std::stoul(found->second.data);
		}
		catch (...) {
			COUT_ERROR << "'" << found->second.data << "' not a valid unsigned integer.";
		}

		found->second.has_data = false;
		_send(id_str, common::protocol::string::make(id_str, value));
		return (true);
	}
	return (false);
}

bool spion::Server::had_to_write(const char * id_str, double & value)
{
	auto found = _dico.find(id_str);
	if (found == _dico.end()) {
		_dico.insert(std::pair(std::string(id_str), bucket(bucket::type_t::data)));
		return (false);
	}

	if (found->second.has_data && found->second.type == bucket::type_t::data) {

		try {
			value = std::stod(found->second.data);
		}
		catch (...) {
			COUT_ERROR << "'" << found->second.data << "' not a valid double.";
		}

		found->second.has_data = false;
		_send(id_str, common::protocol::string::make(id_str, value));
		return (true);
	}
	return (false);
}

