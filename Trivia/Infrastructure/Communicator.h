#pragma once

#include "../Handlers/IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <map>
#include <WinSock2.h>

class Communicator
{
private:
	std::map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory& m_handlerFactory;
	SOCKET m_serverSocket;

	void bindAndListen();
	void handleNewClient(SOCKET clientSocket);

public:
	void startHandleRequests();
};
