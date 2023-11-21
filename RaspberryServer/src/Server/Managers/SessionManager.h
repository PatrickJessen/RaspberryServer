#pragma once
#include "../Interfaces/ISession.h"

class SessionManager
{
public:
	SessionManager(ISession<std::string>* session);
	~SessionManager();

public:
	void SendMessageAsync(const std::string& message);
	void ReadMessageAsync();
	void IsAlive() { session->IsAlive(); }
	const bool GetIsAlive() { return session->isAlive; }
	const SessionType& GetSessionType();
	virtual Message<std::string>* GetMessageObj() { return session->GetMessageObj(); }
private:
	ISession<std::string>* session = nullptr;
};