#ifndef COMMON_NET_TYPES_HH_INCLUDED
#define COMMON_NET_TYPES_HH_INCLUDED



namespace common
{
namespace net
{



using socket_handler_t = int;
using port_t = unsigned short;

static constexpr socket_handler_t not_a_socket = (socket_handler_t)(-1);



} // net
} // common


#endif  // COMMON_NET_TYPES_HH_INCLUDED
