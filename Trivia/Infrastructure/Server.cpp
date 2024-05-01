#include "Server.h"
#include "SqliteDatabase.h"
#include <iostream>
#include <string>
#include <thread>

Server::Server()
    : m_database(new SqliteDatabase()), m_handlerFactory(RequestHandlerFactory(m_database)), m_communicator(Communicator(this->m_handlerFactory))
{}

Server::~Server()
{
    delete this->m_database;
}

enum command
{
    EXIT,
    CLS,
    INVALID_COMMAND
};

static constexpr command hashCommands(const std::string_view& cmd)
{
    if (cmd == "EXIT") return EXIT;
    if (cmd == "cls") return CLS;
    return INVALID_COMMAND;
}

void Server::run()
{
    std::thread t_connector(&Communicator::startHandleRequests, &(this->m_communicator));
    t_connector.detach();

    std::string userInput;
    command cmd;
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
            std::cerr << "Invalid command\n";
            break;
        default:
            break;
        }
    } while (cmd != EXIT);
}
