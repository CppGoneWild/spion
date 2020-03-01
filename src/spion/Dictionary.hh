#ifndef SPION_DICTIONARY_HH_INCLUDED
#define SPION_DICTIONARY_HH_INCLUDED



#include <map>
#include <string>



namespace spion
{


struct bucket
{
	enum class type_t { data, hook };

	bucket(type_t t) : data(), type(t), has_data(false) {}
	bucket(std::string const & d, type_t t) : data(d), type(t), has_data(true) {}

	std::string data;
	type_t type;
	bool has_data;
};

using Dictionary = std::map<std::string, bucket>;



} // spion



#endif  // SPION_DICTIONARY_HH_INCLUDED
