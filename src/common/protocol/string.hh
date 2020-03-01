#ifndef COMMON_PROTOCOL_STRING_HH_INCLUDED
#define COMMON_PROTOCOL_STRING_HH_INCLUDED



#include "types.hh"


#include <string>
#include <array>



namespace common
{
namespace protocol
{
namespace string
{



constexpr char telnet_end[3] = "\r\n";
constexpr std::size_t telnet_end_size = 2;


payload make(const char * id_str, int);
payload make(const char * id_str, unsigned int);
payload make(const char * id_str, double);
payload make(const char * id_str, char const *);
payload make(const char * str);


template <class SOCK_T>
std::string on_recv(SOCK_T &, payload & partial_msg_buffer);

std::string payload_to_string(payload::iterator first, payload::iterator last);

std::string extract_telnet_string(payload & src);



} // string
} // protocol
} // common



#include "string.ipp"



#endif  // COMMON_PROTOCOL_STRING_HH_INCLUDED
