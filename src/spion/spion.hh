#ifndef SPION_HH_INCLUDED
#define SPION_HH_INCLUDED



#include <string>



namespace spion
{



bool init(unsigned short port_number);
void clean();


void send_ro(char const *);
void send_ro(std::string const &);

void send_ro(const char * id_str, int);
void send_ro(const char * id_str, unsigned int);
void send_ro(const char * id_str, double);
void send_ro(const char * id_str, char const *);



} // spion


#endif  // SPION_HH_INCLUDED
