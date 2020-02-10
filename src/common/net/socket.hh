#ifndef COMMON_NET_SOCKET_HH_INCLUDED
#define COMMON_NET_SOCKET_HH_INCLUDED



#include "types.hh"



namespace common
{
namespace net
{



// RAII to close a socket
// Used server side and client side to send and receive data
// Only tcp-ip/ipv4
class socket
{
public:
	socket() = default;

	socket(common::net::socket &&);
	socket & operator=(common::net::socket &&);

	~socket();

	explicit socket(socket_handler_t);

	bool operator==(socket_handler_t const &) const;
	bool operator!=(socket_handler_t const &) const;

	bool operator==(socket const &) const;
	bool operator!=(socket const &) const;

	bool is_ok() const;
	operator bool () const;

	int send(void const * msg, int len);
	int recv(void * buffer, int len);

	void close();

	socket_handler_t handler() const;
	socket_handler_t & handler();

private:
	socket(socket const &)             = delete;
	socket & operator=(socket const &) = delete;

	socket_handler_t _socket = not_a_socket;
};



} // net
} // common



#endif  // COMMON_NET_SOCKET_HH_INCLUDED
