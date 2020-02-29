#ifndef COMMON_PROTOCOL_STRING_HH_INCLUDED
#define COMMON_PROTOCOL_STRING_HH_INCLUDED



#include "types.hh"


#include <string>



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
std::string on_recv(SOCK_T &, std::string & partial_msg_buffer);

std::string extract_telnet_string(std::string & src);

// std::string extract(payload const &, std::string & value);



} // string
} // protocol
} // common



#include "string.ipp"



#endif  // COMMON_PROTOCOL_STRING_HH_INCLUDED
