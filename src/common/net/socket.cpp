#include "socket.hh"
#include "scoped_addrinfo.hh"


#include <sys/socket.h>
#include <unistd.h>


#include <cassert>



/*
                                                  __        __
                                 _________  _____/ /_____  / /_
                                / ___/ __ \/ ___/ //_/ _ \/ __/
                               (__  ) /_/ / /__/ ,< /  __/ /_
                              /____/\____/\___/_/|_|\___/\__/
                              
*/



common::net::socket::socket(common::net::socket && oth)
: _socket(oth._socket)
{
	oth._socket = not_a_socket;
}

common::net::socket & common::net::socket::operator=(common::net::socket && oth)
{
	if (&oth != this) {
		_socket = oth._socket;
		oth._socket = not_a_socket;
	}
	return (*this);
}

common::net::socket::~socket()
{
	if (_socket != not_a_socket)
		::close(_socket);
}

common::net::socket::socket(socket_handler_t s)
: _socket(s)
{}

bool common::net::socket::operator==(socket_handler_t const & oth) const
{
	return (_socket == oth);
}

bool common::net::socket::operator!=(socket_handler_t const & oth) const
{
	return (_socket != oth);
}

bool common::net::socket::operator==(socket const & oth) const
{
	return (_socket == oth._socket);
}

bool common::net::socket::operator!=(socket const & oth) const
{
	return (_socket != oth._socket);
}

bool common::net::socket::is_ok() const
{
	return (_socket != not_a_socket);
}

common::net::socket::operator bool () const
{
	return (is_ok());
}

int common::net::socket::send(void const * msg, int len)
{
	assert(is_ok());

	return (::send(_socket, msg, len, 0));
}

int common::net::socket::recv(void * buffer, int len)
{
	assert(is_ok());

	return (::recv(_socket, buffer, len, 0));
}

void common::net::socket::close()
{
	if (_socket != not_a_socket)
		::close(_socket);
	_socket = not_a_socket;
}

common::net::socket_handler_t common::net::socket::handler() const
{
	return (_socket);
}

common::net::socket_handler_t & common::net::socket::handler()
{
	return (_socket);
}
