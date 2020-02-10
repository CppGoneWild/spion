#ifndef COMMON_NET_CLIENT_HH_INCLUDED
#define COMMON_NET_CLIENT_HH_INCLUDED



#include "socket.hh"
#include "poller.hh"


#include <chrono>



namespace common
{
namespace net
{



class client
{
public:
	client()                      = default;
	client(client &&)             = default;
	client & operator=(client &&) = default;
	~client()                     = default;

	bool is_ok() const;
	operator bool () const;

	bool connect(char const * addr, port_t port);

	int send(void const * msg, int len);
	int recv(void * buffer, int len);

	void close();

	bool poll(std::chrono::milliseconds);

private:
	client(client const &)             = delete;
	client & operator=(client const &) = delete;

	socket _socket;
	single_poller _poller;
};



} // net
} // common



#endif  // COMMON_NET_CLIENT_HH_INCLUDED
