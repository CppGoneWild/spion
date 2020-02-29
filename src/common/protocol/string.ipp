namespace common
{
namespace protocol
{
namespace string
{



static inline bool is_telnet_end(std::string const & str)
{
	auto found = str.find("\r\n");
	return (found != std::string::npos);
}

template <class SOCK_T>
std::string on_recv(SOCK_T & sock)
{
	static constexpr int read_size = 128;

	char buffer[read_size + 1];
	int red;
	std::string tmp;

	for (;;)
	{
		red = sock.recv(buffer, read_size);

		if (red <= 0)
			break; // error or disconnection

		buffer[red] = '\0';
		tmp += buffer;

		if (is_telnet_end(tmp))
			break;
	}

	return (tmp);
}



/*
template <class SOCK_T>
std::string extract(payload const & payload,
                    std::string & value)
{
	std::size_t tk_start = std::string::npos;
	std::size_t tk_end   = std::string::npos;

	auto _extract_and_advance = [&payload, &tk_start, &tk_end](char s, char e)
	{
		tk_start = payload.find(s, (tk_end == std::string::npos ? 0 : tk_end + 1));
		tk_end = payload.find(e, tk_start);

		if (tk_start != std::string::npos)
			return (payload.substr(tk_start + 1, tk_end));
		return (std::string(""));
	};


	id_str.clear();
	value.clear();


	// extract id
	id_str = _extract_and_advance('[', ']');

	// extract value
	tk_start = payload.find('=', (tk_end == std::string::npos ? 0 : tk_end + 1));
	if (tk_start != std::string::npos)
		value = payload.substr(tk_start + 1);
	else
		value = payload.substr((tk_end == std::string::npos ? 0 : tk_end + 1));

	return (id_str);
}
*/


} // string
} // protocol
} // common
