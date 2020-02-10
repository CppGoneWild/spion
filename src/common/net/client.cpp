#include "client.hh"
#include "connect.hh"


bool common::net::client::is_ok() const
{
	return (_socket.is_ok());
}

common::net::client::operator bool () const
{
	return (is_ok());
}

bool common::net::client::connect(char const * addr, port_t port)
{
	bool res = common::net::connect(_socket, addr, port);
	if (res)
		_poller.set(_socket);
	return (res);
}

int common::net::client::send(void const * msg, int len)
{
	return (_socket.send(msg, len));
}

int common::net::client::recv(void * buffer, int len)
{
	return (_socket.recv(buffer, len));
}

void common::net::client::close()
{
	_socket.close();
	_poller.unset();
}

bool common::net::client::poll(std::chrono::milliseconds t)
{
	return (_poller.poll(t));
}

