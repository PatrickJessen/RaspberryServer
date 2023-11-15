#pragma once
#include <thread>
#include "Session.h"

class Server
{
public:
	Server(io_service& service, int port);
	~Server() = default;

	void Start();
	void Stop();

private:
	void AcceptSession();
	void MessageThread();
	void CheckForDisconnection();
private:
	io_service& service;
	tcp::acceptor acceptor;
	std::vector<std::shared_ptr<Session>> sessions;
	Message<std::string>* message = nullptr;
	int nextId = 0;
	bool running = false;
	std::thread message_thread;
	std::thread disconnect_thread;
};