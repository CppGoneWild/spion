#ifndef COMMON_NET_LISTENER_HH_INCLUDED
#define COMMON_NET_LISTENER_HH_INCLUDED



#include "socket.hh"



namespace common
{
namespace net
{



// Used server side to wait for incoming connection on a determined port
// Only tcp-ip/ipv4
class listener
{
public:
	listener()                        = default;
	listener(listener &&)             = default;
	listener & operator=(listener &&) = default;
	~listener()                       = default;

	bool operator==(socket_handler_t const &) const;
	bool operator!=(socket_handler_t const &) const;

	bool operator==(socket const &) const;
	bool operator!=(socket const &) const;

	bool operator==(listener const &) const;
	bool operator!=(listener const &) const;

	bool is_ok() const;
	operator bool() const;

	bool init(port_t);
	socket accept();

	void close();

	socket_handler_t handler() const;
	socket_handler_t & handler();

private:
	listener(listener const &)             = delete;
	listener & operator=(listener const &) = delete;

	socket _socket;

	friend class poller; // direct acces to _socket;
};



} // net
} // common



#endif  // COMMON_NET_LISTENER_HH_INCLUDED
