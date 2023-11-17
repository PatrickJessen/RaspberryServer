#pragma once
#include <string>
#include "ISession.h"

class IServer
{
public:
	virtual void Connect(const std::string& ipAdress, const int& port) = 0;
	virtual void Disconnect() = 0;
	virtual void ListenForConnectionAsync() = 0;
	virtual void HandleDisconnection() = 0;
	virtual void HandleMessages() = 0;
public:
	std::vector<ISession<void*>*> sessions;
	bool running;
};