#pragma once
#include "../Interfaces/IServer.h"


class ServerManager
{
public:
	ServerManager(IServer* server);
	~ServerManager();

public:
	void Connect(const std::string& ipAdress, const int& port);
	void Disconnect();

private:
	void ListenForConnectionAsync();
	void HandleMessages();

private:
	std::shared_ptr<IServer> server = nullptr;
	bool running = false;
	std::thread message_thread;
};