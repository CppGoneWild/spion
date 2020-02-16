#ifndef COMMON_PROTOCOL_TYPES_HH_INCLUDED
#define COMMON_PROTOCOL_TYPES_HH_INCLUDED



#include <cstdint>




namespace common
{
namespace protocol
{



enum class data_type : std::uint8_t
{
	_None   = 0,
	_Int    = 1,
	_UInt   = 2,
	_Double = 3,
	_String = 4,
	_Blob   = 5,

	_Dico = 6
};



} // protocol
} // common

#endif  // COMMON_PROTOCOL_TYPES_HH_INCLUDED
