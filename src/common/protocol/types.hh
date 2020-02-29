#ifndef COMMON_PROTOCOL_TYPES_HH_INCLUDED
#define COMMON_PROTOCOL_TYPES_HH_INCLUDED



#include <vector>



namespace common
{
namespace protocol
{



using payload = std::vector<char>;



template <class SOCK_T>
bool send(SOCK_T & sock, payload const & payload)
{
	return (sock.send(payload.data(), payload.size()));
}



} // protocol
} // common



#endif  // COMMON_PROTOCOL_TYPES_HH_INCLUDED
