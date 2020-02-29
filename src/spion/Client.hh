#ifndef SPION_CLIENT_HH_INCLUDED
#define SPION_CLIENT_HH_INCLUDED



#include "common/net/socket.hh"
#include "common/protocol/string.hh"

#include <list>
#include <regex>


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
	bool send(common::protocol::payload const &);


	recv_event on_recv();
	recv_event recv(std::string &);
	bool execute_remote_cmd(std::string const &);

	void close();

	common::net::socket const & socket() const;
	common::net::socket & socket();

private:
	Client(Client const &)             = delete;
	Client & operator=(Client const &) = delete;

	common::net::socket _socket;
	std::string _buffer;

	std::list<std::regex> _listening_id;
};


} // spion


#endif  // SPION_CLIENT_HH_INCLUDED
