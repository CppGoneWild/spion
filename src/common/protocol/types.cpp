#include "types.hh"

#include <cmath>



common::protocol::double_data::double_data(double v)
{
	int exponant;
	double significant = std::frexp(v, &exponant);

	significant_part = (std::int32_t)(significant * precision);
	exponant_part = exponant;
}

double common::protocol::double_data::as_double() const
{
	double tmp = (double)(significant_part) * precision;
	return (std::ldexp(tmp, exponant_part));
}
