#ifndef COMMON_SCOPED_ADDRINFO_HH_INCLUDED
#define COMMON_SCOPED_ADDRINFO_HH_INCLUDED



extern "C"
{
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <netdb.h>
}



namespace common
{
namespace net
{



// Internal for listener and connect

////////////////////////////////////////////////////////////////////////////////
//                              SCOPED_ADDRINFO                               //
// Use RAII to ensure call to freeaddrinfo() if internal pointer is not null. //
////////////////////////////////////////////////////////////////////////////////
class scoped_addrinfo
{
public:
	scoped_addrinfo() = default;

	~scoped_addrinfo();

	struct addrinfo * get();
	struct addrinfo ** gget();

private:
	scoped_addrinfo(scoped_addrinfo const &)             = delete;
	scoped_addrinfo(scoped_addrinfo &&)                  = delete;
	scoped_addrinfo & operator=(scoped_addrinfo const &) = delete;
	scoped_addrinfo & operator=(scoped_addrinfo &&)      = delete;

	struct addrinfo *_addr = nullptr;
};



} // net
} // common



#endif  // COMMON_SCOPED_ADDRINFO_HH_INCLUDED
