#include "SessionManager.h"

SessionManager::SessionManager(ISession* session)
	: session(session)
{
}

SessionManager::~SessionManager()
{
	delete session;
}

void SessionManager::SendMessageAsync(const std::string& message)
{
	session->SendMessageAsync(message);
}

void SessionManager::ReadMessageAsync()
{
	session->ReadMessageAsync();
}

const SessionType& SessionManager::GetSessionType()
{
	return session->type;
}
