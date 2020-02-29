#ifndef SPION_SHELL_HH_INCLUDED
#define SPION_SHELL_HH_INCLUDED



#include <vector>
#include <string>
#include <functional>



namespace spion
{


class Shell
{
public:
	struct command
	{
		std::string command_full;
		std::string command_abrv;
		std::function<void(std::vector<std::string> const &)> func;

		bool operator==(std::string const & str)
		{
			return (str == command_abrv || str == command_full);
		}
	};

	Shell()         = default;
	Shell(Shell &&) = default;
	~Shell()        = default;

	Shell & operator=(Shell &&) = default;

	bool exec(std::string const &);

	void add_command(command &&);

private:
	Shell(Shell const &)             = default;
	Shell & operator=(Shell const &) = default;

	std::vector<command> _commands;
};



} // spion



#endif  // SPION_SHELL_HH_INCLUDED
