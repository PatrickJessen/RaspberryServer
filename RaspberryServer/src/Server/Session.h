#pragma once
#include <boost/asio.hpp>
#include "Message.h"

using namespace boost::asio;
using namespace boost::asio::ip;

enum class Component { NONE, APP, MICROCONTROLLER };
class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(int id, tcp::socket socket);
	~Session() = default;

	void SendMsg(const std::string& message);
	void DoRead();
	void OnConnected();

	Component GetComponentType() { return component; }
	const int& GetId() { return id; }
	Message<std::string>* GetMessages() { return message; }
	void SetConnectionStatus(const bool status) { connected = status; }
private:
	tcp::socket m_socket;
	int id;
	Component component = Component::NONE;
	enum {BUFFER_SIZE = 1024};
	char data[BUFFER_SIZE];
	bool connected = false;
	Message<std::string>* message = nullptr; // TODO: USE THIS IN SERVER.H INSTEAD OF HERE (take a message reference in DoRead() parameters)
};