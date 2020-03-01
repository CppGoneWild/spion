#include "Client.hh"

#include <algorithm>


#include "logg.hh"


spion::Client::Client(common::net::socket && socket, Dictionary & dico)
: _socket(std::move(socket)),
  _partial_buffer(),
  _shell(),
  _listening_id()
{
	std::function<std::string(std::vector<std::string> const &)> read_cmd =
	[this](std::vector<std::string> const & cmd) -> std::string
	{
		std::string res;
		for (auto it = cmd.begin() + 1; it != cmd.end(); ++it) {
			this->_listening_id.push_back(std::regex(*it));
			res += "Reading '" + *it + "'" + "\n";
		}
		return (res);
	};
	_shell.add_command(Shell::command { "read", "r", "Listen to a variable. Use regex.", read_cmd } );

	std::function<std::string(std::vector<std::string> const &)> write_cmd =
	[&dico](std::vector<std::string> const & cmd) -> std::string
	{
		if (cmd.size() < 3)
			return ("write:syntax error     id_name value");

		auto found = dico.find(cmd[1]);
		if (found == dico.end()){
			dico.insert(std::pair(cmd[1], bucket(cmd[2], bucket::type_t::data)));
		}
		else if (found->second.type == bucket::type_t::data) {
			found->second.data = cmd[2];
			found->second.has_data = true;
		}
		else{
			return (std::string("write: " + cmd[1] + " is not a data type."));
		}
		return ("");
	};
	_shell.add_command(Shell::command { "write", "w", "Write to a variable.", write_cmd } );

	std::function<std::string(std::vector<std::string> const &)> ls_cmd =
	[&dico](std::vector<std::string> const & cmd) -> std::string
	{
		std::string res("list of all variable and hook");
		for (auto it = dico.cbegin(); it != dico.cend(); ++it)
			res += "\n" + it->first;
		return (res);
	};
	_shell.add_command(Shell::command { "ls", "", "List all variable and hook.", ls_cmd } );

	std::string welcome_str;
	_shell.exec({"help"}, welcome_str);
	send(common::protocol::string::make(welcome_str.c_str()));
}

bool spion::Client::operator==(common::net::socket_handler_t oth) const
{
	return (_socket == oth);
}

bool spion::Client::operator!=(common::net::socket_handler_t oth) const
{
	return (_socket != oth);
}

bool spion::Client::operator==(Client const & oth) const
{
	return (_socket == oth._socket);
}

bool spion::Client::operator!=(Client const & oth) const
{
	return (_socket != oth._socket);
}

bool spion::Client::is_ok() const
{
	return (_socket.is_ok());
}

spion::Client::operator bool () const
{
	return (is_ok());
}

bool spion::Client::is_listening_to(const char * str_id)
{
	for (auto it = _listening_id.cbegin(); it != _listening_id.cend(); ++it)
		if (std::regex_search(str_id, *it))
			return (true);
	return (false);
}

bool spion::Client::send(common::protocol::payload const & payload)
{
	return (common::protocol::send(_socket, payload));
}

spion::Client::recv_event spion::Client::on_recv()
{
	std::string received = common::protocol::string::on_recv(_socket, _partial_buffer);

	if (received.empty())
		return(recv_event::disconnection);

	do
	{
		std::string answer;
		if (_shell.exec(received, answer) == false)
			COUT_INFO << received;
		else if (answer.empty() == false)
			send(common::protocol::string::make(answer.c_str()));

		received = common::protocol::string::extract_telnet_string(_partial_buffer);
	} while (received.empty() == false);

	return(recv_event::payload);
}

void spion::Client::close()
{
	_socket.close();
}

common::net::socket const & spion::Client::socket() const
{
	return (_socket);
}

common::net::socket & spion::Client::socket()
{
	return (_socket);
}

