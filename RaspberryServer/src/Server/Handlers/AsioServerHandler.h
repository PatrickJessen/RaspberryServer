#pragma once
#include <boost/asio.hpp>
#include "../Interfaces/IServer.h"
#include "AsioSessionHandler.h"

using namespace boost::asio;
using namespace boost::asio::ip;

class AsioServerHandler : public IServer
{
public:
	AsioServerHandler(int port);
	~AsioServerHandler();
public:
	virtual void Connect(const std::string& ipAdress, const int& port) override;
	virtual void Disconnect() override;
	virtual void ListenForConnectionAsync() override;
private:
	int nextId = 0;
};