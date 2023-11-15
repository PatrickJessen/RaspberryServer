#include "Session.h"
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <iostream>

Session::Session(int id, tcp::socket socket)
    : m_socket(std::move(socket)), id(id)
{
    message = new Message<std::string>();
}

void Session::SendMsg(const std::string& message)
{
    try
    {
        auto self(shared_from_this());
        boost::asio::async_write(
            m_socket, buffer(message, message.size()),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    
                }
            });
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
    }
}

void Session::DoRead()
{
    try
    {
        auto self(shared_from_this());
        m_socket.async_read_some(buffer(data, BUFFER_SIZE),
            [this, self](boost::system::error_code ec,
                std::size_t length) {
                    if (!ec) {
                        if (data != "") {
                            message->PushBack(std::string(data, length));
                            if (!message->GetQueue().empty()) {
                                std::cout << "Message received from id " << id << ": " << message->GetQueue().back().c_str() << std::endl;
                            }
                            if (!connected) {
                                component = (Component)atoi(message->GetQueue().back().c_str());
                                if (component == Component::APP) {
                                    std::cout << "APP joined the server\n";
                                    component = Component::APP;
                                    message->AddOwner((int)Component::MICROCONTROLLER);
                                }
                                else if (component == Component::MICROCONTROLLER) {
                                    std::cout << "MICROCONTROLLER joined the server\n";
                                    component = Component::MICROCONTROLLER;
                                    message->AddOwner((int)Component::APP);
                                }
                                connected = true;
                                // Start thread to check if socket is still alive
                                std::thread(&Session::OnConnected, this).detach();
                            }
                        }
                        DoRead();
                    }
            });
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
    }
}

void Session::OnConnected()
{
    while (connected)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        try
        {
            streambuf buf;
            read_until(m_socket, buf, "");
        }
        catch (std::exception e)
        {
            connected = false;
        }
    }
}
