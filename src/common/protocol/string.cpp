#include "string.hh"



static std::string to_string(common::protocol::data_type type)
{
	if (type == common::protocol::data_type::_Int)
		return ("int");
	if (type == common::protocol::data_type::_UInt)
		return ("uint");
	if (type == common::protocol::data_type::_Double)
		return ("double");
	if (type == common::protocol::data_type::_String)
		return ("string");
	return ("-unknow-");
}

static common::protocol::data_type from_string(std::string const & str)
{
	if (str == "int")
		return (common::protocol::data_type::_Int);
	if (str == "uint")
		return (common::protocol::data_type::_UInt);
	if (str == "double")
		return (common::protocol::data_type::_Double);
	if (str == "string")
		return (common::protocol::data_type::_String);
	return (common::protocol::data_type::_None);
}

static common::protocol::string::payload _make(common::protocol::data_type type,
                                               const char * id_str,
                                               std::string const & value)
{
	common::protocol::string::payload str;

	str += "(";
	str += to_string(type);
	str += ") [";
	str += id_str;
	str += "] =";
	str += value;

	// telnet end of line
	str += '\r';
	str += '\n';

	return (str);
}

common::protocol::string::payload common::protocol::string::make(const char * id_str, int value)
{
	return (_make(data_type::_Int, id_str, std::to_string(value)));
}

common::protocol::string::payload common::protocol::string::make(const char * id_str, unsigned int value)
{
	return (_make(data_type::_UInt, id_str, std::to_string(value)));
}

common::protocol::string::payload common::protocol::string::make(const char * id_str, double value)
{
	return (_make(data_type::_Double, id_str, std::to_string(value)));
}

common::protocol::string::payload common::protocol::string::make(const char * id_str, char const * value)
{
	return (_make(data_type::_String, id_str, std::string(value)));
}
