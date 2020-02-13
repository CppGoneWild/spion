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
	packet_header_data(id_t, data_type, std::uint16_t);

	id_t           id;
	data_type      type;
	std::uint16_t	 size;	
};


using packet = std::vector<char>;

packet make_packet(id_t, int);
packet make_packet(id_t, unsigned int);
packet make_packet(id_t, double);
packet make_packet(id_t, char const *);
packet make_packet(id_t, void const *, std::size_t);



void const * extract_packet(packet const &, packet_header_data const **);



} // protocol
} // common



#endif  // COMMON_PROTOCOL_PACKET_HH_INCLUDED
