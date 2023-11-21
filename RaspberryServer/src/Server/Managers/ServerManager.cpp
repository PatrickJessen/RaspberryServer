#include "ServerManager.h"
#include <iostream>

ServerManager::ServerManager(IServer* server)
	: server(server)
{
}

ServerManager::~ServerManager()
{
	
}

void ServerManager::Connect(const std::string& ipAdress, const int& port)
{
    running = true;
    ListenForConnectionAsync();
    message_thread = std::thread(&ServerManager::HandleMessages, this);
	server->Connect(ipAdress, port);
}

void ServerManager::Disconnect()
{
	server->Disconnect();
    message_thread.join();
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
    while (server->running) {
        for (int i = 0; i < server->sessions.size(); i++) {
            try
            {
                HandleDisconnection();
                Message<std::string>* msg = server->sessions[i]->GetMessageObj();
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
                                std::cout << "ServerManager::HandleMessages() Exception caught: Failed to send message. " << e.what() << "\n";
                            }
                        }
                    }
                }
            }
            catch (std::exception e)
            {
                std::cout << "ServerManager::HandleMessages() Exception caught: " << e.what() << "\n";
            }
        }
    }
}
