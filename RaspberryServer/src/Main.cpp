#include "Server/Managers/ServerManager.h"
#include "Server/Handlers/AsioServerHandler.h"

int main()
{
	int port = 9999;
	boost::asio::io_service service;
	ServerManager* server = new ServerManager(new AsioServerHandler(service, port));
	server->Connect("127.0.0.1", port);
	return 0;
}