#include "Communicator.h"
#include "RequestHandlerFactory.h"
#include "Server.h"
#include "ServerDefinitions.h"
#include "SqliteDatabase.h"
#include <iostream>
#include <string>
#include <string_view>
#include <thread>


Server::Server() :
    m_database(SqliteDatabase::getInstance()),
    m_handlerFactory(RequestHandlerFactory::getInstance()),
    m_communicator(Communicator::getInstance())
{}

Server::~Server() noexcept
{}

enum command
{
    EXIT,
    CLS,
    INVALID_COMMAND
};

static constexpr command hashCommands(const std::string_view cmd) noexcept
{
    if (cmd == "exit") return EXIT;
    if (cmd == "cls") return CLS;
    return INVALID_COMMAND;
}

void Server::run()
{
    std::thread t_connector(&Communicator::startHandleRequests, this->m_communicator);
    t_connector.detach();

    std::string userInput;
    command cmd{};
    do
    {
        std::cin >> userInput;
        cmd = hashCommands(userInput);
        switch (cmd)
        {
        case CLS:
            system("cls");
            break;
        case INVALID_COMMAND:
            std::cerr << ANSI_RED << "Invalid command\n" << ANSI_NORMAL;
            break;
        case EXIT:
            break;
        }
    } while (cmd != EXIT);
}

// Singleton
Server& Server::getInstance()
{
    static Server instance; // This is thread-safe in C++11 and later
    return instance;
}
