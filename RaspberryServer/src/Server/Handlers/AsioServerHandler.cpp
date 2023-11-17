#include "AsioServerHandler.h"
#include "../Managers/SessionManager.h"
#include "AsioSessionHandler.h"
#include <iostream>

static std::mutex session_mutex;

AsioServerHandler::AsioServerHandler(io_service& service, int port)
	: service(service), acceptor(service, tcp::endpoint(tcp::v4(), port))
{
    running = false;
}

void AsioServerHandler::Connect(const std::string& ipAdress, const int& port)
{
	service.run();
    ListenForConnectionAsync();
    message_thread = std::thread(&AsioServerHandler::HandleMessages, this);
    running = true;
}

void AsioServerHandler::Disconnect()
{
    service.stop();
    message_thread.join();
    running = false;
    sessions.clear();
}

void AsioServerHandler::ListenForConnectionAsync()
{
    try
    {
        if (running)
        {
            acceptor.async_accept(
                [this](boost::system::error_code ec, tcp::socket socket) {
                    if (!ec) {
                        auto session = std::make_shared<SessionManager>(new AsioSessionHandler(nextId, std::move(socket)));
                        nextId++;
                        session->ReadMessageAsync();
                        {
                            std::lock_guard<std::mutex> lock(session_mutex);
                            sessions.push_back(session);
                        }
                    }
            ListenForConnectionAsync();
                });
        }
    }
    catch (std::exception e)
    {
        std::cout << "AsioServerHandler::ListenForConnectionAsync() Exception caught: " << e.what() << "\n";
    }
}

void AsioServerHandler::HandleDisconnection()
{
    sessions.erase(std::remove_if(sessions.begin(), sessions.end(),
        [](const auto& session) { return !session->GetIsAlive(); }),
        sessions.end());
}

void AsioServerHandler::HandleMessages()
{
    while (running) {
        for (int i = 0; i < sessions.size(); i++) {
            try
            {
                HandleDisconnection();
                Message<std::string>* msg = (Message<std::string>*)sessions[i]->GetMessageObj();
                if (msg->GetQueue().empty()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    continue;
                }

                for (int k = 0; k < sessions.size(); k++) {
                    for (int j = 0; j < msg->GetOwners().size(); j++) {
                        if (sessions[k]->GetSessionType() == (SessionType)msg->GetOwners()[j]) {
                            try
                            {
                                if (!msg->GetQueue().empty()) {
                                    std::string text = msg->GetQueue().front();
                                    sessions[k]->SendMessageAsync(text);
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
