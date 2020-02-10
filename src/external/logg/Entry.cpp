#include "Entry.hh"

#include "I_Output.hh"


//                    ______      __
//                   / ____/___  / /________  __
//                  / __/ / __ \/ __/ ___/ / / /
//                 / /___/ / / / /_/ /  / /_/ /
//                /_____/_/ /_/\__/_/   \__, /
//                                     /____/


logg::Entry::Entry(Entry && oth)
: _out(oth._out), _meta_data(oth._meta_data), _data(std::move(oth._data))
{
	oth._out = nullptr;
}


logg::Entry::Entry(I_Output & out, MetaData const & mdata)
: _out(&out), _meta_data(mdata), _data()
{}

logg::Entry::~Entry()
{
	if (_out && !_data.empty())
		_out->_log(*this);
	_data.clear();
	_out = nullptr;
}

logg::Entry & logg::Entry::operator=(Entry && oth)
{
	_out = oth._out;
	oth._out = nullptr;
	_meta_data = oth._meta_data;
	_data = std::move(oth._data);
	return (*this);
}

void logg::Entry::append(std::string const & str)
{
	_data += str;
}

void logg::Entry::append(std::string && str)
{
	_data += str;
}

void logg::Entry::append(char const * str)
{
	_data += str;
}

logg::MetaData const & logg::Entry::meta_data() const
{
	return (_meta_data);
}

std::string const & logg::Entry::data() const
{
	return (_data);
}


//                                       __             ____
//          ____  ____  ___  _________ _/ /_____  _____/ / /
//         / __ \/ __ \/ _ \/ ___/ __ `/ __/ __ \/ ___/ / /
//        / /_/ / /_/ /  __/ /  / /_/ / /_/ /_/ / /   \ \ \
//        \____/ .___/\___/_/   \__,_/\__/\____/_/     \_\_\
//            /_/


logg::Entry operator<<(logg::Entry && entry, std::string const & str)
{
	entry.append(str);
	return (std::move(entry));
}

logg::Entry operator<<(logg::Entry && entry, std::string && str)
{
	entry.append(str);
	return (std::move(entry));
}

logg::Entry operator<<(logg::Entry && entry, char const * str)
{
	entry.append(str);
	return (std::move(entry));
}
