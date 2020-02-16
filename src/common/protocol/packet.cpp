#include "packet.hh"


#include <cstring>
#include <cassert>



common::protocol::packet_header_data::packet_header_data(id_t id, data_type t, std::uint16_t s)
: id(id), type(t), size(s)
{}



static common::protocol::packet _make_packet(common::protocol::packet_header_data const & header, void const * data)
{
	common::protocol::packet p;
	p.reserve(sizeof(common::protocol::packet_header_data) + header.size);

	for (std::size_t i = 0; i < sizeof(common::protocol::packet_header_data); ++i)
		p.push_back(((char const *)(&header))[i]);

	for (std::size_t i = 0; i < header.size; ++i)
		p.push_back(((char const *)(data))[i]);

	return (p);
}



common::protocol::packet common::protocol::make_packet(id_t id, int value)
{
	std::int32_t tmp = value;
	packet_header_data header(id, data_type::_Int, sizeof(tmp));

	return (_make_packet(header, &tmp));	
}

common::protocol::packet common::protocol::make_packet(id_t id, unsigned int value)
{
	std::uint32_t tmp = value;
	packet_header_data header(id, data_type::_UInt, sizeof(tmp));

	return (_make_packet(header, &tmp));	
}

common::protocol::packet common::protocol::make_packet(id_t id, double value)
{
	double_data double_encoder(value);
	packet_header_data header(id, data_type::_Double, sizeof(double_encoder));

	return (_make_packet(header, &double_encoder));	
}

common::protocol::packet common::protocol::make_packet(id_t id, char const * value)
{
	static_assert(sizeof(char) == 1, "need to upgrade serialisation for string");

	assert(value);

	packet_header_data header(id, data_type::_String, std::strlen(value));

	return (_make_packet(header, &value));	
}

common::protocol::packet common::protocol::make_packet(id_t id, void const * value, std::size_t size)
{
	assert(value);
	assert(size);

	packet_header_data header(id, data_type::_Blob, size);

	return (_make_packet(header, &value));	
}











void const * common::protocol::extract_packet(packet const & p, packet_header_data const ** header)
{
	void const * payload = nullptr;

	assert(header);

	if (p.size() >= sizeof(packet_header_data)) {
		*header = (packet_header_data const *)(p.data());
	}
	else {
		assert(false); // packet too small.
		*header = nullptr;
		return (nullptr);
	}

	if (p.size() != sizeof(packet_header_data) + (*header)->size) {
		assert(false); // invalid packet size.
		return (nullptr);
	}

	if ((*header)->size > 0)
		payload = p.data() + sizeof(packet_header_data);		

	return (payload);
}
