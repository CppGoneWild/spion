namespace common
{
namespace protocol
{
namespace string
{



template <class SOCK_T>
std::string on_recv(SOCK_T & sock, std::string & partial_msg_buffer)
{
	static constexpr int read_size = 128;
	char buffer[read_size + 1];
	int red;

	for (;;)
	{
		std::string msg = extract_telnet_string(partial_msg_buffer);
		if (msg.empty() == false)
			return (msg);

		red = sock.recv(buffer, read_size);

		if (red <= 0)
			break; // error or disconnection

		buffer[red] = '\0';
		partial_msg_buffer += buffer;
	}

	return (std::move(partial_msg_buffer));
}



} // string
} // protocol
} // common
