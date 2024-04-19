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
	void startHandleRequests();

private:
	std::map<SOCKET, IRequestHandler*> m_clients;
	//RequestHandlerFactory& m_handlerFactory;
	SOCKET m_serverSocket;

	void bindAndListen();
	void serverListen();
	void acceptClient();
	void handleNewClient(SOCKET clientSocket);
};
