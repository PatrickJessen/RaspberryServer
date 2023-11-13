#pragma once
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

enum class Component { APP = 1, MICROCONTROLLER };
class Session
{
public:
	Session(Component component, tcp::socket socket);
	Session() {}
	~Session() = default;
};