#include "string.hh"


static common::protocol::payload _make(std::string const & value)
{
	std::string str(value);

	// telnet end of line
	str += common::protocol::string::telnet_end;

	common::protocol::payload res;
	res.reserve(str.size() + 1);
	res.insert(res.end(), str.begin(), str.end());

	return (res);
}

static common::protocol::payload _make(const char * id_str, std::string const & value)
{
	std::string str;

	str += "[";
	str += id_str;
	str += "] =";
	str += value;

	return (_make(str));
}

common::protocol::payload common::protocol::string::make(const char * id_str, int value)
{
	return (_make(id_str, std::to_string(value)));
}

common::protocol::payload common::protocol::string::make(const char * id_str, unsigned int value)
{
	return (_make(id_str, std::to_string(value)));
}

common::protocol::payload common::protocol::string::make(const char * id_str, double value)
{
	return (_make(id_str, std::to_string(value)));
}

common::protocol::payload common::protocol::string::make(const char * id_str, char const * value)
{
	return (_make(id_str, std::string(value)));
}

common::protocol::payload common::protocol::string::make(char const * value)
{
	return (_make(std::string(value)));
}





std::string common::protocol::string::extract_telnet_string(std::string & src)
{
	std::string result;
	auto found = src.find(telnet_end);

	if (found != std::string::npos) {
		result = src.substr(0, found);
		src.erase(0, result.size() + telnet_end_size);
	}

	return (result);
}
