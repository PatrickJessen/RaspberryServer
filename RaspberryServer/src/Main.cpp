#include "Server/Managers/ServerManager.h"
#include "Server/Handlers/AsioServerHandler.h"

int main()
{
	int port = 9999;
	ServerManager* server = new ServerManager(new AsioServerHandler(port));
	server->Connect("127.0.0.1", port);
	//server->Connect("192.168.0.24", port);
	return 0;
}