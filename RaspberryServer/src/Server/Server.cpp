#include "Server.h"
#include <iostream>
#include "Message.h"

static std::mutex session_mutex;

Server::Server(io_service& service, int port)
    : service(service), acceptor(service, tcp::endpoint(tcp::v4(), port))
{
    message = new Message<std::string>();
}

void Server::Start()
{
    running = true;
    AcceptSession();
    message_thread = std::thread(&Server::MessageThread, this);
}

void Server::Stop()
{
    running = false;
    message_thread.join();
}

void Server::AcceptSession()
{
    if (running) {
        try
        {
            acceptor.async_accept(
                [this](boost::system::error_code ec, tcp::socket socket) {
                    if (!ec) {
                        auto session = std::make_shared<Session>(nextId, std::move(socket));
                        nextId++;
                        session->DoRead();
                        {
                            std::lock_guard<std::mutex> lock(session_mutex);
                            sessions.push_back(session);
                        }
                    }
            AcceptSession();
                });
        }
        catch (std::exception e)
        {
            std::cout << e.what() << "\n";
        }
    }
}

void Server::MessageThread()
{
    while (running) {
        for (int i = 0; i < sessions.size(); i++) {
            try
            {
                /*if (CheckForDisconnection(i)) {
                    continue;
                }*/
                Message<std::string>* msg = sessions[i]->GetMessages();
                if (msg->GetQueue().empty()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    continue;
                }
                
                for (int k = 0; k < sessions.size(); k++) {
                    for (int j = 0; j < msg->GetOwners().size(); j++) {
                        if (sessions[k]->GetComponentType() == (Component)msg->GetOwners()[j]) {
                            try
                            {
                                if (!msg->GetQueue().empty()) {
                                    std::string text = msg->GetQueue().front();
                                    sessions[k]->SendMsg(text);
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

            }
        }
    }
}

const bool Server::CheckForDisconnection(const int& i)
{
    try
    {
        if (sessions[i] != nullptr) {
            if (!sessions[i]->IsConnected()) {
                if (sessions[i]->GetComponentType() == Component::APP) {
                    std::cout << "APP disconnected\n";
                }
                else if (sessions[i]->GetComponentType() == Component::MICROCONTROLLER) {
                    std::cout << "MICROCONTROLLER disconnected\n";
                }
                sessions.erase(sessions.begin() + i);
                return true;
            }
        }
        else {
            std::cout << "Error: " << i << "\n";
        }
        return false;
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
        sessions.erase(sessions.begin() + i);
        return true;
    }
    return false;
}
