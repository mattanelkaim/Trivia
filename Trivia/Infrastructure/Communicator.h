#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <mutex>
#include <unordered_map>
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
    static Communicator* getInstance(RequestHandlerFactory* handlerFactory);

private:
    // Members
    std::unordered_map<SOCKET, IRequestHandler*> m_clients;
    RequestHandlerFactory* m_handlerFactory;
    SOCKET m_serverSocket;

    // Private methods
    void bindAndListen() const;
    void handleNewClient(SOCKET clientSocket) noexcept;
    void disconnectClient(const SOCKET clientSocket) noexcept;

    // Singleton
    explicit Communicator(RequestHandlerFactory* handlerFactory);
    ~Communicator() noexcept;
    inline static Communicator* m_Communicator = nullptr;
    inline static std::mutex m_mutex;
};
