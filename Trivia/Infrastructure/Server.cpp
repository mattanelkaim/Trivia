#include "Communicator.h"
#include "RequestHandlerFactory.h"
#include "Server.h"
#include "SqliteDatabase.h"
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>


Server::Server() :
    m_database(new SqliteDatabase()),
    m_handlerFactory(RequestHandlerFactory::getInstance(m_database)),
    m_communicator(Communicator::getInstance(m_database))
{}

Server::~Server() noexcept
{
    delete this->m_database;
}

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
            std::cerr << "Invalid command\n";
            break;
        case EXIT:
            break;
        }
    } while (cmd != EXIT);
}

// Singleton
Server* Server::getInstance()
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    if (m_Server == nullptr)
    {
        m_Server = std::shared_ptr<Server>(new Server());
    }
    return m_Server.get();
}
