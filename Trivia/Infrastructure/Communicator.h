#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <map>
#include <WinSock2.h>

class Communicator
{
public:
    explicit Communicator(RequestHandlerFactory& handlerFactory);
    Communicator() = delete;
    ~Communicator() noexcept;
    void startHandleRequests(); // Why is it public?

private:
    std::map<SOCKET, IRequestHandler*> m_clients;
    RequestHandlerFactory& m_handlerFactory;
    SOCKET m_serverSocket;

    void bindAndListen() const;
    void handleNewClient(SOCKET clientSocket);
};
