#include "Shell.hh"

#include <algorithm>


static std::vector<std::string> parse(std::string const & cmd)
{
	std::vector<std::string> res;

	std::size_t start = 0;
	std::size_t end;

	do
	{
		end = cmd.find(" ", start);

		auto tmp = cmd.substr(start, end - start);
		if (tmp.empty() == false)
			res.push_back(tmp);
		
		start = end + 1;
	
	} while (end != std::string::npos && start < cmd.size());

	return (res);
}

spion::Shell::Shell()
{
	std::function<std::string(std::vector<std::string> const &)> help_cmd =
	[this](std::vector<std::string> const & cmd) -> std::string
	{
		if (cmd.size() > 1) {
			auto found = std::find(this->_commands.begin(), this->_commands.end(), cmd[1]);
			if (found != this->_commands.end())
				return (found->help_short);
			return (std::string("help: Command '") + cmd[1] + "' does not exist.");
		}

		std::string all("list of all command:");
		for (auto it = this->_commands.cbegin(); it != this->_commands.cend(); ++it)
			all += "\n" + it->command_full + "\t" + it->command_abrv + + "\t" + it->help_short;
		return (all);
	};

	add_command(command { "help", "", "Display help for the command or list all commands.", help_cmd } );
}


bool spion::Shell::exec(std::string const & cmd, std::string & res)
{
	std::vector<std::string> parameters = parse(cmd);

	if (parameters.empty() == false)
	{
		auto found = std::find(_commands.begin(), _commands.end(), parameters[0]);
		if (found != _commands.end()) {
			res = found->func(parameters);
			return (true);
		}
	}

	return (false);
}

void spion::Shell::add_command(command && cmd)
{
	_commands.push_back(cmd);
}

