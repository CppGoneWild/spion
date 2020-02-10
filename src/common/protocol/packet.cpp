#include "packet.hh"



common::protocol::packet common::protocol::make_packet(packet_header_data const & header, void const * data)
{
	packet p;
	p.reserve(sizeof(packet_header_data) + header.size);

	for (std::size_t i = 0; i < sizeof(packet_header_data); ++i)
		p.push_back(((char const *)(&header))[i]);

	for (std::size_t i = 0; i < header.size; ++i)
		p.push_back(((char const *)(data))[i]);

	return (p);
}

void const * common::protocol::extract_packet(packet const & p, packet_header_data const ** header)
{
	void const * payload = nullptr;

	if (p.size() >= sizeof(packet_header_data))
		*header = (packet_header_data const *)(p.data());
	else
		*header = nullptr;

	if (*header && (*header)->size > 0)
		payload = p.data() + sizeof(packet_header_data);

	return (payload);
}
