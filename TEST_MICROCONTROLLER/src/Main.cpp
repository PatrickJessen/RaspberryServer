#include <boost/asio.hpp>
#include <iostream>
#include "SensorManager.h"
#include "TemperatureSensor.h"

using namespace boost::asio;
using namespace boost::asio::ip;

enum class Component
{
    NONE,
    APP,
    MICROCONTROLLER
};

std::string message = "";

void getData(tcp::socket& socket)
{
    while (true)
    {
        try
        {

            message = "";
            streambuf buf;
            read_until(socket, buf, "\n");
            std::string data = buffer_cast<const char*>(buf.data());
            message = data;
            std::cout << "Received message: " << message << std::endl;
        }
        catch (std::exception e)
        {
            std::cout << e.what() << "\n";
        }
    }
}

void sendData(tcp::socket& socket, const std::string& message)
{
    write(socket, buffer(message + "\n"));
}

int main(int argc, char* argv[])
{
    SensorManager<float>* sensor = new SensorManager<float>(new TemperatureSensor());


    io_service io_service;
    ip::tcp::socket client_socket(io_service);
    client_socket.connect(tcp::endpoint(address::from_string("127.0.0.1"), 9999));
    bool connected = true;

    std::string validationBody = "{secret_code}";

    sendData(client_socket, std::to_string((int)Component::MICROCONTROLLER));
    std::string response;

    std::thread(&getData, std::ref(client_socket)).detach();
    float temp = 0.0f;
    while (true) {
        try
        {
            sensor->Poll();
            float currentTemp = sensor->Read();
            if (temp != currentTemp) {
                temp = currentTemp;
                std::string msg = validationBody;
                msg.append(std::to_string(temp));
                sendData(client_socket, msg);
                std::cout << "Send message: " << msg << "\n";
            }
        }
        catch (std::exception e)
        {
            std::cout << "server is offline\n";
            client_socket.close();
            io_service.stop();
            connected = false;
            while (!connected)
            {
                std::cout << "Reconnecting...\n";
                try
                {
                    client_socket.connect(tcp::endpoint(address::from_string("127.0.0.1"), 9999));
                    std::cout << "Reconnected\n";
                    connected = true;
                }
                catch (std::exception e)
                {
                    std::cout << "Failed to reconnect\n";
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            }
        }
    }
    client_socket.close();
    return 0;
}