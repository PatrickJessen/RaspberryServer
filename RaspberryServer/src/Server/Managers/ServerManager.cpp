#include "ServerManager.h"
#include <iostream>

ServerManager::ServerManager(IServer* server)
	: server(server)
{
}

ServerManager::~ServerManager()
{
	delete server;
}

void ServerManager::Connect(const std::string& ipAdress, const int& port)
{
	server->Connect(ipAdress, port);
}

void ServerManager::Disconnect()
{
	server->Disconnect();
}

void ServerManager::ListenForConnectionAsync()
{
	server->ListenForConnectionAsync();
}

void ServerManager::HandleDisconnection()
{
	server->HandleDisconnection();
}

void ServerManager::HandleMessages()
{
    while (running) {
        for (int i = 0; i < server->sessions.size(); i++) {
            try
            {
                HandleDisconnection();
                Message<std::string>* msg = (Message<std::string>*)server->sessions[i]->GetMessageObj();
                if (msg->GetQueue().empty()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    continue;
                }

                for (int k = 0; k < server->sessions.size(); k++) {
                    for (int j = 0; j < msg->GetOwners().size(); j++) {
                        if (server->sessions[k]->GetSessionType() == (SessionType)msg->GetOwners()[j]) {
                            try
                            {
                                if (!msg->GetQueue().empty()) {
                                    std::string text = msg->GetQueue().front();
                                    server->sessions[k]->SendMessageAsync(text);
                                    msg->PopFront();
                                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                                }
                            }
                            catch (std::exception e)
                            {

                            }
                        }
                    }
                }
            }
            catch (std::exception e)
            {
                std::cout << "AsioServerHandler::HandleMessages() Exception caught: " << e.what() << "\n";
            }
        }
    }
}
