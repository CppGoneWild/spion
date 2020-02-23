#include "Client.hh"

#include <algorithm>
#include <cstring>


#include "logg.hh"


spion::Client::Client(common::net::socket && socket)
: _socket(std::move(socket)),
  _buffer(),
  _listening_id()
{}

bool spion::Client::operator==(common::net::socket_handler_t oth) const
{
	return (_socket == oth);
}

bool spion::Client::operator!=(common::net::socket_handler_t oth) const
{
	return (_socket != oth);
}

bool spion::Client::operator==(Client const & oth) const
{
	return (_socket == oth._socket);
}

bool spion::Client::operator!=(Client const & oth) const
{
	return (_socket != oth._socket);
}

bool spion::Client::is_ok() const
{
	return (_socket.is_ok());
}

spion::Client::operator bool () const
{
	return (is_ok());
}

bool spion::Client::is_listening_to(const char * str_id)
{
	for (auto it = _listening_id.cbegin(); it != _listening_id.cend(); ++it)
		if (std::regex_search(str_id, *it))
			return (true);
	return (false);
}

bool spion::Client::send(common::protocol::string::payload const & payload)
{
	return (common::protocol::string::send(_socket, payload));
}

spion::Client::recv_event spion::Client::recv(common::protocol::string::payload & result)
{
	_buffer += common::protocol::string::on_recv(_socket);

	if (_buffer.empty())
		return(recv_event::disconnection);

	auto found = _buffer.find("\r\n");
	if (found != std::string::npos) {
		result = _buffer.substr(0, found);
		_buffer.erase(0, result.size() + 2);
	}
	else {
		result = std::move(_buffer);
	}

	return(recv_event::payload);
}

bool spion::Client::execute_remote_cmd(std::string const & cmd)
{
	auto listen_cmd = [this](std::string const & cmd) -> bool
	{
		auto found = cmd.find("l ");
		if (found == std::string::npos)
			return (false);

		auto id = cmd.substr(found + std::strlen("l "));
		this->_listening_id.push_back(std::regex(id));

		COUT_INFO << "Listen to " << id;

		return (true);
	};

	return (listen_cmd(cmd));
}

void spion::Client::close()
{
	_socket.close();
}

common::net::socket const & spion::Client::socket() const
{
	return (_socket);
}

common::net::socket & spion::Client::socket()
{
	return (_socket);
}

