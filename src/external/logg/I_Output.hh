#ifndef LOGG_I_OUTPUT_HH__
#define LOGG_I_OUTPUT_HH__


#include "Entry.hh"


namespace logg
{


class I_Output
{
public:
	virtual ~I_Output() = default;

	virtual Entry log(Level_e lvl,
	                  char const * file,
	                  std::size_t line,
	                  char const * func);

protected:
	I_Output()                 = default;
	I_Output(I_Output const &) = default;
	I_Output(I_Output &&)      = default;

	I_Output & operator=(I_Output const &) = default;
	I_Output & operator=(I_Output &&)      = default;

	virtual void _log(Entry const &) = 0;

	Entry create_entry(MetaData && meta_data);

	friend class Entry;
};


} // logg


#endif // LOGG_I_OUTPUT_HH__
