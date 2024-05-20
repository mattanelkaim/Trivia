#pragma once

#include "IDatabase.h"
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <memory>
#include <mutex>
#include <unordered_map>
#include <WinSock2.h>

class Communicator final
{
public:
    // Public methods
    void startHandleRequests(); // Why is it public?

    // Singleton
    Communicator() = delete;
    Communicator(Communicator& other) = delete;
    void operator=(const Communicator& other) = delete;
    static Communicator* getInstance(IDatabase* db);
    ~Communicator() noexcept;

private:
    // Members
    RequestHandlerFactory& m_handlerFactory;
    SOCKET m_serverSocket;
    std::unordered_map<SOCKET, IRequestHandler*> m_clients;

    // Private methods
    void bindAndListen() const;
    void handleNewClient(SOCKET clientSocket);
    void disconnectClient(SOCKET clientSocket) noexcept;

    // Singleton
    explicit Communicator(IDatabase* db);
    inline static std::shared_ptr<Communicator> m_Communicator = nullptr;
    inline static std::mutex m_mutex;
};
