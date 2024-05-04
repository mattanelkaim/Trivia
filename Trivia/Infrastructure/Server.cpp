#include "Server.h"
#include "SqliteDatabase.h"
#include <iostream>
#include <string>
#include <thread>

// Singleton
Server* m_Server = nullptr;
std::mutex Server::m_mutex;


Server* Server::getInstance()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_Server == nullptr)
    {
        m_Server = new Server;
    }
    return m_Server;
}

Server::Server()
    : m_database(new SqliteDatabase()),
      m_handlerFactory(RequestHandlerFactory::getInstance(m_database)),
      m_communicator(Communicator::getInstance(m_handlerFactory))
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

static constexpr command hashCommands(const std::string_view cmd)
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
        case EXIT:
        default:
            break;
        }
    } while (cmd != EXIT);
}
