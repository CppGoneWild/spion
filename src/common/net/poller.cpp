#include "poller.hh"
#include "listener.hh"
#include "socket.hh"


#include <cassert>



/*
                                      ____
                         ____  ____  / / /__  _____
                        / __ \/ __ \/ / / _ \/ ___/
                       / /_/ / /_/ / / /  __/ /
                      / .___/\____/_/_/\___/_/
                     /_/
*/



void common::net::poller::add(socket_handler_t s)
{
	assert(s != not_a_socket);

	struct pollfd tmp;
	tmp.fd = s;
	tmp.events = POLLIN;
	_pollfd.push_back(tmp);
}

void common::net::poller::remove(socket_handler_t s)
{
	for (auto it = _pollfd.begin(); it != _pollfd.end(); ++it)
		if (it->fd == s) {
			_pollfd.erase(it);
			return ;
		}
}

void common::net::poller::add(socket & s)
{
	assert(s.handler() != not_a_socket);
	add(s.handler());
}

void common::net::poller::remove(socket & s)
{
	remove(s.handler());
}

void common::net::poller::add(listener & l)
{
	add(l._socket);
}

void common::net::poller::remove(listener & l)
{
	remove(l._socket);
}

std::vector<common::net::socket_handler_t> common::net::poller::poll(std::chrono::milliseconds time)
{
	std::vector<common::net::socket_handler_t> res;

	const int num_events = ::poll(_pollfd.data(),
	                              _pollfd.size(),
	                              time.count());

	int found = 0;
	for (auto it = _pollfd.begin(); it != _pollfd.end() && found < num_events; ++it)
		if (it->revents & POLLIN) {
			res.push_back(it->fd);
			++found;
		}

	return (res);
}

