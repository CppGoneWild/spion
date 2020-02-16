#include <cstring>



namespace common
{
namespace protocol
{
namespace packet
{



template <class SOCK_T>
bool set_protocol(SOCK_T & sock)
{
	return (sock.send(magic_string_for_packet_mode,
	                  std::strlen(magic_string_for_packet_mode)));
}

template <class SOCK_T>
bool send(SOCK_T & sock, payload const & p)
{
	return (sock.send(p.data(), p.size()));
}

template <class SOCK_T>
header_data const * on_recv(SOCK_T & sock, payload & payload, void const ** data)
{}




} // packet
} // protocol
} // common

