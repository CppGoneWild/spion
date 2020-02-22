#include "packet.hh"


#include <cstring>
#include <cassert>
#include <cmath>



char const common::protocol::packet::magic_string_for_packet_mode[] = "spion-protocol=packet_mode_v1";
char const common::protocol::packet::magic_string_for_string_mode[8] = "pstring";



common::protocol::packet::double_payload_encoder::double_payload_encoder(double v)
{
	int exponant;
	double significant = std::frexp(v, &exponant);

	significant_part = (std::int32_t)(significant * precision);
	exponant_part = exponant;
}

double common::protocol::packet::double_payload_encoder::as_double() const
{
	double tmp = (double)(significant_part) * precision;
	return (std::ldexp(tmp, exponant_part));
}






common::protocol::packet::header_data::header_data(id_t id, data_type t, std::uint16_t s)
: type(t), id(id), size(s)
{}





static common::protocol::packet::payload _make_paylaod(common::protocol::packet::header_data const & header, void const * data)
{
	common::protocol::packet::payload p;
	p.reserve(sizeof(common::protocol::packet::header_data) + header.size);

	for (std::size_t i = 0; i < sizeof(common::protocol::packet::header_data); ++i)
		p.push_back(((char const *)(&header))[i]);

	for (std::size_t i = 0; i < header.size; ++i)
		p.push_back(((char const *)(data))[i]);

	return (p);
}



common::protocol::packet::payload common::protocol::packet::make(id_t id, int value)
{
	std::int32_t tmp = value;
	header_data header(id, data_type::_Int, sizeof(tmp));

	return (_make_paylaod(header, &tmp));	
}

common::protocol::packet::payload common::protocol::packet::make(id_t id, unsigned int value)
{
	std::uint32_t tmp = value;
	header_data header(id, data_type::_UInt, sizeof(tmp));

	return (_make_paylaod(header, &tmp));	
}

common::protocol::packet::payload common::protocol::packet::make(id_t id, double value)
{
	double_payload_encoder double_encoder(value);
	header_data header(id, data_type::_Double, sizeof(double_encoder));

	return (_make_paylaod(header, &double_encoder));	
}

common::protocol::packet::payload common::protocol::packet::make(id_t id, char const * value)
{
	static_assert(sizeof(char) == 1, "need to upgrade serialisation for string");

	assert(value);

	header_data header(id, data_type::_String, std::strlen(value));

	return (_make_paylaod(header, &value));	
}

common::protocol::packet::payload common::protocol::packet::make(id_t id, void const * value, std::size_t size)
{
	assert(value);
	assert(size);

	header_data header(id, data_type::_Blob, size);

	return (_make_paylaod(header, &value));	
}
