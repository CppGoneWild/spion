#ifndef COMMON_NET_CONNECT_HH_INCLUDED
#define COMMON_NET_CONNECT_HH_INCLUDED



#include "types.hh"
#include "socket.hh"



namespace common
{
namespace net
{



// Used client side to initialise a socket connected to a distant host
// Only tcp-ip/ipv4
socket connect(char const * addr, port_t port);



} // net
} // common



#endif  // COMMON_NET_CONNECT_HH_INCLUDED
