#include "Server.h"
#include <iostream>
#include <string>
#include <thread>

constexpr std::string_view EXIT = "EXIT";

Server::Server()
    : m_database(nullptr) {}

void Server::run()
{
    std::thread t_connector(&Communicator::startHandleRequests, &(this->m_communicator));
    t_connector.detach();

    std::string userInput;
    do
    {
        std::cin >> userInput;
    } while (userInput != EXIT);
}
