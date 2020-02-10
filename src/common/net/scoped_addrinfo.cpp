#include "scoped_addrinfo.hh"


#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>



common::net::scoped_addrinfo::~scoped_addrinfo()
{
	if (_addr != nullptr)
		::freeaddrinfo(_addr);
}

struct addrinfo * common::net::scoped_addrinfo::get()
{
	return (_addr);
}

struct addrinfo ** common::net::scoped_addrinfo::gget()
{
	return (&_addr);
}
