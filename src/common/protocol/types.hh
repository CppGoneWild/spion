#ifndef COMMON_PROTOCOL_TYPES_HH_INCLUDED
#define COMMON_PROTOCOL_TYPES_HH_INCLUDED



#include <cstdint>




namespace common
{
namespace protocol
{



using id_t = std::uint16_t;

enum class data_type : std::uint8_t
{
	_Int    = 1,
	_UInt   = 2,
	_Double = 3,
	_String = 4,
	_Blob   = 5,

	_Dico = 6
};

struct double_data
{
	static constexpr double precision = 10000.0;

	double_data(double v);
	double as_double() const;

	std::int32_t significant_part;
	std::int32_t exponant_part;
};



} // protocol
} // common

#endif  // COMMON_PROTOCOL_TYPES_HH_INCLUDED
