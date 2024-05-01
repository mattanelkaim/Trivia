#include "Server.h"
#include "SqliteDatabase.h"
#include <iostream>
#include <string>
#include <thread>

constexpr std::string_view EXIT = "EXIT";

Server::Server()
    : m_database(new SqliteDatabase()), m_handlerFactory(RequestHandlerFactory(m_database)), m_communicator(Communicator(this->m_handlerFactory))
{}

Server::~Server()
{
    delete this->m_database;
}

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
