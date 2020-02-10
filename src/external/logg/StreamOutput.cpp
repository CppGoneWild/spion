#include "StreamOutput.hh"

#include <iostream> // std::cout


static std::string _make_prefix(logg::Entry const & e)
{
	std::string prefix;

	prefix += '[';
	prefix += to_string(e.meta_data().level()) ;
	prefix += ']';
	prefix += ' ';
	prefix += e.meta_data().file();
	prefix += ':';
	prefix += std::to_string(e.meta_data().line());
	prefix += ' ';
	prefix += e.meta_data().function();
	prefix += ' ';

	return (prefix);
}

static const char * _make_prefix_color(logg::Level_e lvl)
{
	if (lvl == logg::Warning)
		return ("\e[1;33m");     // bright;orange  -> yellow
	else if (lvl == logg::Error)
		return ("\e[33m");       // orange
	else if (lvl == logg::Fatal)
		return ("\e[31m");       // red
	else if (lvl == logg::Profil)
		return ("\e[32m");       // green
	else if (lvl == logg::User)
		return ("\e[34m");       // blue
	else
		return ("\e[0m");        // reset all;
}


logg::StreamOutput logg::cout = logg::StreamOutput(std::cout, true);
logg::StreamOutput logg::cerr = logg::StreamOutput(std::cerr, true);
logg::StreamOutput logg::clog = logg::StreamOutput(std::clog, true);


logg::StreamOutput::StreamOutput(std::ostream & s, bool color_mode)
: _stream(&s), _color_mode(color_mode)
{}

void logg::StreamOutput::_log(Entry const & e)
{
	auto compute_size_to_print = [] (std::size_t l_left, std::size_t l_util)
	{
		return ((l_left > l_util) ? (l_util) : (l_left));
	};

	static constexpr std::size_t TERM_WIDTH = 180;

	const bool reset_term_color = (_color_mode && e.meta_data().level() >= logg::Warning);
	const std::string prefix(_make_prefix(e));
	const std::size_t util_size = TERM_WIDTH - prefix.size();

	if (reset_term_color)
		*_stream << _make_prefix_color(e.meta_data().level());

	*_stream << prefix;

	if (e.data().size() < util_size)
		*_stream << e.data() << std::endl;
	else
		for (std::size_t writed = 0; writed < e.data().size(); writed += util_size) {
			if (writed != 0)
				*_stream << std::string(prefix.size(), ' ');
			_stream->write(e.data().c_str() + writed,
			               compute_size_to_print(e.data().size() - writed, util_size));
			*_stream << std::endl;
		}

	if (reset_term_color)
		*_stream << "\e[0m";    // reset all;
}
