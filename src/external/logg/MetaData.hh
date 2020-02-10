#ifndef LOGG_METADATA_HH__
#define LOGG_METADATA_HH__


#include <string>


namespace logg
{


enum Level_e {
	Info = 0,
	Log,
	Warning,
	Error,
	Fatal,
	Profil,
	User,
	_MAX
};

std::string const & to_string(Level_e);


class MetaData
{
public:
	MetaData()                 = default;
	MetaData(MetaData const &) = default;
	MetaData(MetaData &&)      = default;
	~MetaData()                = default;

	MetaData & operator=(MetaData const &) = default;
	MetaData & operator=(MetaData &&)      = default;

	MetaData(Level_e lvl, char const * file, std::size_t line, char const * func);

	Level_e level() const;
	char const * file() const;
	std::size_t line() const;
	char const * function() const;

private:
	Level_e _level = Info;
	char const * _file = nullptr;
	std::size_t _line = 0;
	char const * _function = nullptr;
};


} // logg


#endif // LOGG_METADATA_HH__
