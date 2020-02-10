#ifndef COMMON_PROTOCOL_PACKET_HH_INCLUDED
#define COMMON_PROTOCOL_PACKET_HH_INCLUDED



#include "types.hh"


#include <cstdint>
#include <vector>



namespace common
{
namespace protocol
{



struct packet_header_data
{
	id_t           id;
	data_type      type;
	std::uint16_t	 size;	
};


using packet = std::vector<char>;

packet make_packet(packet_header_data const &, void const * data);
void const * extract_packet(packet const &, packet_header_data const **);



} // protocol
} // common



#endif  // COMMON_PROTOCOL_PACKET_HH_INCLUDED
