#pragma once
#include "../Interfaces/ISession.h"

class SessionManager
{
public:
	SessionManager(ISession<void*>* session);
	~SessionManager();

public:
	void SendMessageAsync(const std::string& message);
	void ReadMessageAsync();
	void IsAlive() { session->IsAlive(); }
	const bool GetIsAlive() { return session->isAlive; }
	const SessionType& GetSessionType();
	virtual Message<void*>* GetMessageObj() { return session->GetMessageObj(); }
private:
	ISession<void*>* session = nullptr;
};