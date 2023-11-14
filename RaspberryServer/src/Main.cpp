#include "Server/Server.h"

int main()
{
	boost::asio::io_service service;
	Server* server = new Server(service, 9999);
	server->Start();
	service.run();
	return 0;
}