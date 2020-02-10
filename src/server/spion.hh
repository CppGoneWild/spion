#ifndef SPION_HH_INCLUDED
#define SPION_HH_INCLUDED



#include <cstdint>



namespace spion
{



bool init(unsigned short port_number);
void clean();


void send_ro(char const * str_id, int);
void send_ro(char const * str_id, double);
void send_ro(char const * str_id, unsigned int);
void send_ro(char const * str_id, char const *);
void send_ro(char const * str_id, void const *, std::size_t size);


void send_rw(char const * str_id, int &);
void send_rw(char const * str_id, double &);
void send_rw(char const * str_id, unsigned int &);
void send_rw(char const * str_id, char *);
void send_rw(char const * str_id, void *, std::size_t size);



} // spion


#endif  // SPION_HH_INCLUDED
