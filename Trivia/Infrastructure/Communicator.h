#pragma once

#include "../Handlers/IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <map>
#include <mutex>
#include <WinSock2.h>

class Communicator
{
public:
    // Public methods
    void startHandleRequests(); // Why is it public?

    // Singleton
    Communicator() = delete;
    Communicator(Communicator& other) = delete;
    void operator=(const Communicator& other) = delete;
    static Communicator* getInstance(RequestHandlerFactory& handlerFactory);

private:
    // Members
    std::map<SOCKET, IRequestHandler*> m_clients;
    RequestHandlerFactory& m_handlerFactory;
    SOCKET m_serverSocket;

    // Private methods
    void bindAndListen() const;
    void handleNewClient(SOCKET clientSocket);

    // Singleton
    explicit Communicator(RequestHandlerFactory& handlerFactory);
    ~Communicator() noexcept;
    inline static Communicator* m_Communicator = nullptr;
    static std::mutex m_mutex;
};
