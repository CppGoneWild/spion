#ifndef COMMON_NET_POLL_HH_INCLUDED
#define COMMON_NET_POLL_HH_INCLUDED


#include <vector>
#include <chrono> // milliseconds


#include <poll.h>


#include "types.hh"



namespace common {
namespace net {
	class socket;
	class listener;
} // net
} // common


namespace common
{
namespace net
{



// Used to wait on multiple socket for an amount of time
class poller
{
public:
	poller()                      = default;
	poller(poller &&)             = default;
	poller & operator=(poller &&) = default;
	~poller()                     = default;

	void add(socket_handler_t);
	void remove(socket_handler_t);

	void add(socket &);
	void remove(socket &);

	void add(listener & l);
	void remove(listener & l);

	// WARNING: may have a listener socket in result.
	std::vector<socket_handler_t> poll(std::chrono::milliseconds);

private:
	poller(poller const &)             = delete;
	poller & operator=(poller const &) = delete;

	std::vector<struct pollfd> _pollfd;
};



} // net
} // common



#endif  // COMMON_NET_POLL_HH_INCLUDED
