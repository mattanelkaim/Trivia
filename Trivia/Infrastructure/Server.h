#pragma once

#include "Communicator.h"
#include "IDatabase.h"
#include "RequestHandlerFactory.h"
#include <memory>
#include <mutex>

class Server final
{
public:
    void run();

    // Singleton
    Server(Server& other) = delete;
    void operator=(const Server& other) = delete;
    static std::shared_ptr<Server> getInstance();
    ~Server() noexcept;

private:
    IDatabase* m_database;
    std::shared_ptr<RequestHandlerFactory>& m_handlerFactory;
    Communicator* m_communicator;

    // Singleton
    Server();
    inline static std::shared_ptr<Server> m_Server = nullptr;
    inline static std::mutex m_mutex;
};
