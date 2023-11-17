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
	void ListenForConnectionAsync();
	void HandleDisconnection();
	void HandleMessages();

private:
	IServer* server = nullptr;
	bool running = false;
};