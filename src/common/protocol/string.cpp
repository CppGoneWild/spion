#include "string.hh"


static common::protocol::payload _make(std::string const & value)
{
	std::string str(value);

	// telnet end of line
	str += common::protocol::string::telnet_end;

	common::protocol::payload res;
	res.reserve(str.size());
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





std::string common::protocol::string::payload_to_string(payload::iterator first,
                                                        payload::iterator last)
{
	std::string result;
	for (; first != last; ++first)
		result += *first;
	return (result);
}

std::string common::protocol::string::extract_telnet_string(payload & src)
{
	for (auto it = src.begin(); it != src.end(); ++it)
	{
		if ((it + 1) != src.end() &&  *it == telnet_end[0] && *(it + 1) == telnet_end[1])
		{
			std::string result = payload_to_string(src.begin(), it);
			src.erase(src.begin(), it + telnet_end_size);
			return (result);
		}
	}

	return (std::string());
}
