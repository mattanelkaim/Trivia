#pragma once

#include "Communicator.h"
#include "IDatabase.h"
#include "RequestHandlerFactory.h"
#include <mutex>

class Server
{
public:
    void run();

    // Singleton
    Server(Server& other) = delete;
    void operator=(const Server& other) = delete;
    static Server* getInstance();

private:
    IDatabase* m_database;
    RequestHandlerFactory* m_handlerFactory;
    Communicator* m_communicator;

    // Singleton
    Server();
    ~Server();
    inline static Server* m_Server = nullptr;
    static std::mutex m_mutex;
};
