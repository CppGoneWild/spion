#include "spion.hh"

#include "Server.hh"



static spion::Server & _static_spion_server()
{
	static spion::Server server;
	return (server);
}



bool spion::init(unsigned short port_number)
{
	return (_static_spion_server().init(port_number));
}

void spion::clean()
{
	_static_spion_server().clean();
}


void spion::send_ro(const char * id_str, int value)
{
	_static_spion_server().send_ro(id_str, value);
}

void spion::send_ro(const char * id_str, unsigned int value)
{
	_static_spion_server().send_ro(id_str, value);
}

void spion::send_ro(const char * id_str, double value)
{
	_static_spion_server().send_ro(id_str, value);
}

void spion::send_ro(const char * id_str, char const * value)
{
	_static_spion_server().send_ro(id_str, value);
}


void spion::send_rw(const char * id_str, int & value)
{
	_static_spion_server().send_rw(id_str, value);
}

void spion::send_rw(const char * id_str, unsigned int & value)
{
	_static_spion_server().send_rw(id_str, value);
}

void spion::send_rw(const char * id_str, double & value)
{
	_static_spion_server().send_rw(id_str, value);
}
