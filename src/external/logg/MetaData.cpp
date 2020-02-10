#include "MetaData.hh"


#include <cassert>


//                 __                   __
//                / /   ___ _   _____  / /   ___
//               / /   / _ \ | / / _ \/ /   / _ \
//              / /___/  __/ |/ /  __/ /   /  __/
//             /_____/\___/|___/\___/_/____\___/
//                                   /_____/


std::string const & logg::to_string(Level_e lvl)
{
	static const std::string _names[logg::_MAX] { std::string(" Info  "),
	                                              std::string("  Log  "),
	                                              std::string("Warning"),
	                                              std::string(" Error "),
	                                              std::string(" Fatal "),
	                                              std::string("Profing"),
	                                              std::string(" User  ")
	                                          };
	assert(lvl >= logg::Info && lvl < logg::_MAX);
	return (_names[lvl]);
}


//              __  ___     __        ____        __
//             /  |/  /__  / /_____ _/ __ \____ _/ /_____ _
//            / /|_/ / _ \/ __/ __ `/ / / / __ `/ __/ __ `/
//           / /  / /  __/ /_/ /_/ / /_/ / /_/ / /_/ /_/ /
//          /_/  /_/\___/\__/\__,_/_____/\__,_/\__/\__,_/


logg::MetaData::MetaData(Level_e lvl,
                         char const * file,
                         std::size_t line,
                         char const * func)
: _level(lvl), _file(file), _line(line), _function(func)
{
	assert(lvl >= logg::Info && lvl < logg::_MAX);
}

logg::Level_e logg::MetaData::level() const
{
	return (_level);
}

char const * logg::MetaData::file() const
{
	return (_file);
}

std::size_t logg::MetaData::line() const
{
	return (_line);
}

char const * logg::MetaData::function() const
{
	return (_function);
}
