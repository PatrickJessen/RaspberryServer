#include "AsioSessionHandler.h"
#include "../Managers/ValidatorManager.h"
#include <iostream>

AsioSessionHandler::AsioSessionHandler(int id, tcp::socket socket)
	: socket(std::move(socket))
{
    message = new Message<std::string>();
    this->id = id;
    isAlive = false;
    type = SessionType::NONE;
}

void AsioSessionHandler::SendMessageAsync(const std::string& message)
{
    try
    {
        auto self(this);
        boost::asio::async_write(
            socket, buffer(message, message.size()),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {

                }
            });
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Network Error: " << e.what() << "\n";
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Runtime Error: " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "General Exception: " << e.what() << "\n";
    }
}

void AsioSessionHandler::ReadMessageAsync()
{
    try
    {
        auto self(shared_from_this());
        socket.async_read_some(buffer(data, BUFFER_SIZE),
            [this, self](boost::system::error_code ec,
                std::size_t length) {
                    if (!ec) {
                        ValidatorManager validate;
                        if (validate.ValidateData(data)) {
                            message->PushBack(std::string(data, length));
                            if (!message->GetQueue().empty()) {
                                std::cout << "Message received from id " << id << ": " << message->GetQueue().back().c_str() << std::endl;
                            }
                            if (!isAlive) {
                                type = (SessionType)atoi(message->GetQueue().back().c_str());
                                if (type == SessionType::APP) {
                                    std::cout << "APP joined the server\n";
                                    type = SessionType::APP;
                                    message->AddOwner((int)SessionType::MICROCONTROLLER);
                                }
                                else if (type == SessionType::MICROCONTROLLER) {
                                    std::cout << "MICROCONTROLLER joined the server\n";
                                    type = SessionType::MICROCONTROLLER;
                                    message->AddOwner((int)SessionType::APP);
                                }
                                isAlive = true;
                                // Start thread to check if socket is still alive
                                std::thread(&AsioSessionHandler::IsAlive, this).detach();
                            }
                        }
                        ReadMessageAsync();
                    }
            });
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Network Error: " << e.what() << "\n";
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Runtime Error: " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "General Exception: " << e.what() << "\n";
    }
}

void AsioSessionHandler::IsAlive()
{
    while (isAlive)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        try
        {
            streambuf buf;
            read_until(socket, buf, "");
        }
        catch (std::exception e)
        {
            if (type == SessionType::APP) {
                std::cout << "APP Session Id " << id << " disconnected\n";
            }
            else if (type == SessionType::MICROCONTROLLER) {
                std::cout << "Microcontroller Session Id " << id << " disconnected\n";
            }
            isAlive = false;
        }
    }
}

const SessionType& AsioSessionHandler::GetSessionType()
{
    return type;
}
