#ifndef LOGG_ENTRY_HH__
#define LOGG_ENTRY_HH__


#include "MetaData.hh"


namespace logg {
	class I_Output;
} // logg


namespace logg
{


/**
 * @brief Start point for logg
 * @details Created by I_Output::log(...). Logged at destruction.
 * 
 * @code{.cpp}
 * COUT_WARNING << "some log " << std::to_string(42) << "string only";
 * {
 * 	Entry log_entry = COUT_WARNING;
 * 	log_entry << "will be logged when log_entry will be destroyed";
 * }
 * @endcode
 * 
 */
class Entry
{
public:
	Entry() = default;

	Entry(Entry &&);
	Entry & operator=(Entry &&);

	~Entry();

	void append(std::string const &);
	void append(std::string &&);
	void append(char const *);

	MetaData const & meta_data() const;
	std::string const & data() const;

private:
	// (c++20, T=I_Output is incomplete type)
	// using I_Output_t = std::reference_wrapper<I_Output>;

	Entry(Entry const &)             = delete;
	Entry & operator=(Entry const &) = delete;

	Entry(I_Output & out, MetaData const &);

	I_Output * _out = nullptr;
	MetaData _meta_data;
	std::string _data;

	friend class I_Output;
};


} // logg

logg::Entry operator<<(logg::Entry &&, std::string const &);
logg::Entry operator<<(logg::Entry &&, std::string &&);
logg::Entry operator<<(logg::Entry &&, char const *);

template <class T>
logg::Entry operator<<(logg::Entry && e, T p)
{
	e.append(std::to_string(p));
	return (std::move(e));
}


#endif // LOGG_ENTRY_HH__
