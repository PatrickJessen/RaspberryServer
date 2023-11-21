#pragma once
#include <boost/asio.hpp>
#include "../Interfaces/ISession.h"

using namespace boost::asio;
using namespace boost::asio::ip;

class AsioSessionHandler : public ISession<std::string>
{
public:
	AsioSessionHandler(int id, tcp::socket socket);
public:
	virtual void SendMessageAsync(const std::string& message) override;
	virtual void ReadMessageAsync() override;
	virtual void IsAlive() override;
	virtual const bool GetIsAlive() override { return isAlive; }
	virtual const SessionType& GetSessionType() override;
	virtual Message<std::string>* GetMessageObj() override { return message; }
private:
	tcp::socket socket;
	enum { BUFFER_SIZE = 1024 };
	char data[BUFFER_SIZE]{};
	SessionType type;
	Message<std::string>* message;
	int id;
	bool isAlive;
};