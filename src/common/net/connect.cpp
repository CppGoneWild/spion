#include "connect.hh"
#include "socket.hh"
#include "scoped_addrinfo.hh"


#include <string.h>
#include <sys/socket.h>
#include <errno.h>
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
}



bool common::net::connect(socket & sock_to_connect, char const * addr, port_t port)
{
	assert(sock_to_connect.handler() == not_a_socket); // ensure not already open


	// first, load up address structs with getaddrinfo():
	struct addrinfo hints;
	_init_addr_info(hints);
	std::string port_str(std::to_string(port));

	scoped_addrinfo res;

	int status = ::getaddrinfo(addr, port_str.c_str(), &hints, res.gget());
	if (status  != 0) {
		std::cerr << "getaddrinfo: " << ::gai_strerror(status) << std::endl;
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

		// connect it
		if (::connect(sock, p->ai_addr, p->ai_addrlen) == -1) {
			_print_errno_error("connect");
			::close(sock);
			continue;
		}
		
		sock_to_connect.handler() = sock;
		break;
	}

	return (sock_to_connect.handler() != not_a_socket);
}
