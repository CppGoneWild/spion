#ifndef SPION_HH_INCLUDED
#define SPION_HH_INCLUDED



#include <cstdint>



namespace spion
{



bool init(unsigned short port_number);
void clean();


void send_ro(char const *);



} // spion


#endif  // SPION_HH_INCLUDED
