#pragma once
#include <boost/asio.hpp>
#include "../Interfaces/IServer.h"
#include "../Managers/SessionManager.h"

using namespace boost::asio;
using namespace boost::asio::ip;

class AsioServerHandler : public IServer
{
public:
	AsioServerHandler(io_service& service, int port);
public:
	virtual void Connect(const std::string& ipAdress, const int& port) override;
	virtual void Disconnect() override;
	virtual void ListenForConnectionAsync() override;
	virtual void HandleDisconnection() override;
	virtual void HandleMessages() override;
private:
	std::vector<std::shared_ptr<SessionManager>> sessions;
	io_service& service;
	tcp::acceptor acceptor;
	std::thread message_thread;
	int nextId = 0;
	bool running = false;
};