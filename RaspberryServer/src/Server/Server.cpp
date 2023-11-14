#include "Server.h"
#include <iostream>
#include "Message.h"

static std::mutex session_mutex;
static std::mutex message_mutex;

Server::Server(io_service& service, int port)
    : service(service), acceptor(service, tcp::endpoint(tcp::v4(), port))
{
    
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
        std::lock_guard<std::mutex> message_lock(session_mutex);
        for (int i = 0; i < sessions.size(); i++) {
            try
            {
                Message<std::string>* msg = sessions[i]->GetMessages();
                std::unique_lock<std::mutex> lock(message_mutex);
                if (msg->GetQueue().empty()) {
                    lock.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    continue;
                }
                
                for (int j = 0; j < msg->GetOwners().size(); j++) {
                    if (sessions[i]->GetComponentType() == (Component)msg->GetOwners()[j]) {
                        try
                        {
                            if (!msg->GetQueue().empty()) {
                                sessions[i]->SendMsg(msg->GetQueue().front());
                                msg->PopFront();
                            }
                            std::this_thread::sleep_for(std::chrono::milliseconds(1));
                        }
                        catch (std::exception e)
                        {
                        
                        }
                    }
                }
            }
            catch (std::exception e)
            {

            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}