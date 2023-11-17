#pragma once
#include <string>
#include <vector>
#include "../Message.h"

enum class SessionType { NONE, APP, MICROCONTROLLER };

/// <summary>
/// 
/// </summary>
/// <typeparam name="T">Datatype to send and receive (Currently only handling string)</typeparam>
template <typename T>
class ISession
{
public:
	virtual void SendMessageAsync(const std::string& message) = 0;
	virtual void ReadMessageAsync() = 0;
	/// <summary>
	/// Send read request to session
	/// If request was sucessfull, session is still alive
	/// Otherwise session has disconnected
	/// </summary>
	virtual void IsAlive() = 0;
	virtual const bool GetIsAlive() = 0;
	virtual const SessionType& GetSessionType() = 0;
	virtual Message<T>* GetMessageObj() = 0;
public:
	SessionType type;
	Message<T>* message;
	int id;
	bool isAlive;
};