#include "I_Output.hh"


logg::Entry logg::I_Output::log(Level_e lvl,
                                char const * file,
                                std::size_t line,
                                char const * func)
{
	return (create_entry(MetaData(lvl, file, line, func)));
}

logg::Entry logg::I_Output::create_entry(MetaData && meta_data)
{
	return (logg::Entry(*this, meta_data));
}
