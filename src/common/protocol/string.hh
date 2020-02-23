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



using payload = std::string;

payload make(const char * id_str, int);
payload make(const char * id_str, unsigned int);
payload make(const char * id_str, double);
payload make(const char * id_str, char const *);
payload make(const char * type_str, const char * id_str, const char * str);


template <class SOCK_T>
bool send(SOCK_T &, payload const &);

template <class SOCK_T>
payload on_recv(SOCK_T &);

data_type extract(payload const &, std::string & id_str, std::string & value);



} // string
} // protocol
} // common


#include "string.ipp"


#endif  // COMMON_PROTOCOL_STRING_HH_INCLUDED
