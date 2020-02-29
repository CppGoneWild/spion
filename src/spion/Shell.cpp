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


bool spion::Shell::exec(std::string const & cmd)
{
	std::vector<std::string> parameters = parse(cmd);

	if (parameters.empty() == false)
	{
		auto found = std::find(_commands.begin(), _commands.end(), parameters[0]);
		if (found != _commands.end()) {
			found->func(parameters);
			return (true);
		}
	}

	return (false);
}

void spion::Shell::add_command(command && cmd)
{
	_commands.push_back(cmd);
}

