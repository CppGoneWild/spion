#include "listener.hh"
#include "scoped_addrinfo.hh"



#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>	
#include <unistd.h>


#include <string>
#include <iostream>
#include <cassert>



static void _print_errno_error(const char * context)
{
	static char err_buff[256];
	
	::strerror_r(errno, err_buff, sizeof(err_buff));
	std::cerr << context << ": " << err_buff << std::endl;
}

static void _init_addr_info(struct addrinfo & to_init)
{
	::memset(&to_init, 0, sizeof(to_init));

	to_init.ai_family   = AF_INET;      // use IPv4
	to_init.ai_socktype = SOCK_STREAM;  // TCP-IP
	to_init.ai_flags    = AI_PASSIVE;   // fill in my IP for me
}



/*
                           ___      __
                          / (_)____/ /____  ____  ___  _____
                         / / / ___/ __/ _ \/ __ \/ _ \/ ___/
                        / / (__  ) /_/  __/ / / /  __/ /
                       /_/_/____/\__/\___/_/ /_/\___/_/

*/


bool common::net::listener::operator==(socket_handler_t const & oth) const
{
	return (handler() == oth);
}

bool common::net::listener::operator!=(socket_handler_t const & oth) const
{
	return (handler() != oth);
}

bool common::net::listener::operator==(socket const & oth) const
{
	return (handler() == oth.handler());
}

bool common::net::listener::operator!=(socket const & oth) const
{
	return (handler() != oth.handler());
}

bool common::net::listener::operator==(listener const & oth) const
{
	return (handler() == oth.handler());
}

bool common::net::listener::operator!=(listener const & oth) const
{
	return (handler() != oth.handler());
}

bool common::net::listener::is_ok() const
{
	return (_socket.is_ok());
}

common::net::listener::operator bool() const
{
	return (is_ok());
}

bool common::net::listener::init(port_t port)
{
	assert(handler() == not_a_socket); // ensure not already open

	// first, load up address structs with getaddrinfo():
	struct addrinfo hints;
	_init_addr_info(hints);
	std::string port_str(std::to_string(port));

	scoped_addrinfo res;

	int status = ::getaddrinfo(NULL, port_str.c_str(), &hints, res.gget());
	if (status  != 0) {
		std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
		return (false);
	}

	// loop through all the results
	for(struct addrinfo *p = res.get(); p != NULL; p = p->ai_next) {

		// make a socket
		socket_handler_t sock = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sock == -1) {
			_print_errno_error("socket");
			continue;
		}

		// bind it
		if (::bind(sock, p->ai_addr, p->ai_addrlen) == -1) {
			_print_errno_error("bind");
			::close(sock);
			continue;
		}
		
		handler() = sock;
		break;
	}

	if (handler() == not_a_socket)
		return (false);

	// set port as reusable
	int yes = 1;
	if (::setsockopt(handler(), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		_print_errno_error("setsockopt");
		this->close();
		return (false);
	}


	// listen on it
	static constexpr int backlog_size = 10;
	if (::listen(handler(), backlog_size) == -1)
	{
		_print_errno_error("listen");
		this->close();
		return (false);		
	}

	return (true);
}

common::net::socket common::net::listener::accept()
{
	assert(handler() != not_a_socket); // ensure already open

	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof(their_addr);

	socket_handler_t new_fd = ::accept(handler(),
	                                   (struct sockaddr *)(&their_addr),
	                                   &addr_size);

	if (new_fd == not_a_socket)
		_print_errno_error("accept");

	return (socket(new_fd));
}

void common::net::listener::close()
{
	_socket.close();
}

common::net::socket_handler_t common::net::listener::handler() const
{
	return (_socket.handler());
}

common::net::socket_handler_t & common::net::listener::handler()
{
	return (_socket.handler());
}
