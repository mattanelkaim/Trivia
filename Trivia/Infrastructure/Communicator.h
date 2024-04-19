#pragma once

#include "../Handlers/IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <map>
#include <WinSock2.h>

class Communicator
{
public:
	Communicator();
	~Communicator() noexcept;
	void startHandleRequests(); // Why is it public?

private:
	std::map<SOCKET, IRequestHandler*> m_clients;
	//RequestHandlerFactory& m_handlerFactory;
	SOCKET m_serverSocket;

	void bindAndListen();
	void acceptClient(); // Helper function
	void handleNewClient(SOCKET clientSocket);
};
