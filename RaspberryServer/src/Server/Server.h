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
private:
	io_service& service;
	tcp::acceptor acceptor;
	std::vector<std::shared_ptr<Session>> sessions;
	int nextId = 0;
	bool running = false;
	std::thread message_thread;
};