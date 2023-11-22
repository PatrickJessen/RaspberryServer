#pragma once
#include <string>
#include "ISession.h"

class IServer
{
public:
	virtual ~IServer() = default;
	virtual void Connect(const std::string& ipAdress, const int& port) = 0;
	virtual void Disconnect() = 0;
	virtual void ListenForConnectionAsync() = 0;
public:
	std::vector<std::shared_ptr<ISession<std::string>>> sessions;
	bool running;
};