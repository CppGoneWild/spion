#ifndef SPION_CLIENT_HH_INCLUDED
#define SPION_CLIENT_HH_INCLUDED



#include "common/net/socket.hh"
#include "common/protocol/string.hh"

#include <list>


namespace spion
{


class Client
{
public:
	enum class recv_event { payload, disconnection };

	Client()          = default;
	Client(Client &&) = default;
	~Client()         = default;

	Client & operator=(Client &&) = default;

	explicit Client(common::net::socket && socket);

	bool operator==(common::net::socket_handler_t) const;
	bool operator!=(common::net::socket_handler_t) const;	

	bool operator==(Client const &) const;
	bool operator!=(Client const &) const;	

	bool is_ok() const;
	operator bool () const;

	bool is_listening_to(const char * str_id);
	bool send(common::protocol::string::payload const &);


	recv_event recv(common::protocol::string::payload &);
	bool execute_remote_cmd(std::string const &);

	void close();

	common::net::socket const & socket() const;
	common::net::socket & socket();

private:
	Client(Client const &)             = delete;
	Client & operator=(Client const &) = delete;

	common::net::socket _socket;
	common::protocol::string::payload _buffer;

	std::list<std::string> _listening_id;
};


} // spion


#endif  // SPION_CLIENT_HH_INCLUDED
