namespace common
{
namespace protocol
{
namespace string
{



template <class SOCK_T>
std::string on_recv(SOCK_T & sock, payload & partial_msg_buffer)
{
	static constexpr int read_size = 256;
	std::array<char, read_size> buffer;
	int red;

	for (;;)
	{
		std::string msg = extract_telnet_string(partial_msg_buffer);
		if (msg.empty() == false)
			return (msg);

		red = sock.recv(buffer.data(), buffer.size());

		if (red <= 0)
			break; // error or disconnection

		partial_msg_buffer.insert(partial_msg_buffer.end(), buffer.begin(), buffer.begin() + red);
	}

	std::string result = payload_to_string(partial_msg_buffer.begin(), partial_msg_buffer.end());
	partial_msg_buffer.clear();
	return (result);
}



} // string
} // protocol
} // common
