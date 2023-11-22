#include "AsioServerHandler.h"
#include "AsioSessionHandler.h"
#include <iostream>

static std::mutex session_mutex;
io_service* service;
tcp::acceptor* acceptor;

AsioServerHandler::AsioServerHandler(int port)
{
    service = new io_service();
    acceptor = new tcp::acceptor(*service, tcp::endpoint(tcp::v4(), port));
    running = true;
}

void AsioServerHandler::Connect(const std::string& ipAdress, const int& port)
{
	service->run();
}

void AsioServerHandler::Disconnect()
{
    service->stop();
    running = false;
    sessions.clear();
}

void AsioServerHandler::ListenForConnectionAsync()
{
    if (running)
    {
        try
        {
            acceptor->async_accept(
                [this](boost::system::error_code ec, tcp::socket socket) {
                    if (!ec) {
                        std::shared_ptr<ISession<std::string>> session = std::make_shared<AsioSessionHandler>(nextId, std::move(socket));
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
        catch (std::exception e)
        {
            std::cout << "AsioServerHandler::ListenForConnectionAsync() Exception caught: " << e.what() << "\n";
        }
    }
}
