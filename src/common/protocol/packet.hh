#ifndef COMMON_PROTOCOL_PACKET_HH_INCLUDED
#define COMMON_PROTOCOL_PACKET_HH_INCLUDED



#include "types.hh"


#include <cstdint>
#include <vector>



namespace common
{
namespace protocol
{
namespace packet
{



extern char const magic_string_for_packet_mode[];
extern char const magic_string_for_string_mode[];

/** called by client : set protocol with spion server to packet mode
 *  this operation is definitive
 *  affect only this particular client,
 *   it will not change setting for other clients.
 */
template <class SOCK_T> bool set_protocol_to_packet(SOCK_T &);
template <class SOCK_T> bool set_protocol_to_string(SOCK_T &);



using id_t = std::uint16_t;



struct header_data
{
	header_data(id_t, data_type, std::uint16_t);

	data_type      type;      // 8b  = 1 char		total:8b -1c
	id_t           id;        // 16b = 2 char		total:24b-3c
	std::uint16_t	 size;      // 16b = 2 char		total:40b-5c

	std::uint8_t _padding[3]; // 24b = 3 char		total:64b-8c
};



using payload = std::vector<char>;
struct double_payload_encoder
{
	static constexpr double precision = 10000.0;

	double_payload_encoder(double v);
	double as_double() const;

	std::int32_t significant_part;
	std::int32_t exponant_part;
};



payload make(id_t, int);
payload make(id_t, unsigned int);
payload make(id_t, double);
payload make(id_t, char const *);
payload make(id_t, void const *, std::size_t);


template <class SOCK_T>
bool send(SOCK_T &, payload const &);

template <class SOCK_T>
header_data const * on_recv(SOCK_T &, payload & payload, void const ** data);



} // packet
} // protocol
} // common



#include "packet.ipp"



#endif  // COMMON_PROTOCOL_PACKET_HH_INCLUDED
