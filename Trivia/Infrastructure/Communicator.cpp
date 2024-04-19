#include "Communicator.h"
#include <iostream>
#include <string>
#include <thread>
#include <WinSock2.h>

using std::to_string;
constexpr auto PORT = 7777;

Communicator::Communicator()
{
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (this->m_serverSocket == INVALID_SOCKET)
		throw std::runtime_error(__FUNCTION__ " - socket() err: " + to_string(WSAGetLastError()));
}

void Communicator::bindAndListen() const
{
	struct sockaddr_in serverAddr{};
	serverAddr.sin_family = AF_INET; // Must be AF_INET
	serverAddr.sin_port = htons(PORT); // Port that server will listen for
	serverAddr.sin_addr.s_addr = INADDR_ANY; // When there are few IPs for the machine. We will use always "INADDR_ANY"

	// Connects the socket and the configuration
	if (bind(this->m_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		throw std::runtime_error(__FUNCTION__ " - bind" + to_string(WSAGetLastError()));

	// Start listening for incoming client requests
	if (listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::runtime_error(__FUNCTION__ " - listen" + to_string(WSAGetLastError()));

	std::cout << "Listening on port " << PORT << "...\n";
	std::thread connector(&Communicator::handleNewClient, this);
	connector.detach();
}

void Communicator::handleNewClient(SOCKET clientSocket)
{}


#include "WSAInitializer.h"
int main(void)
{
	try
	{
		WSAInitializer wsaInit;
		Communicator c;
		c.bindAndListen();
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << e.what();
	}

	return 0;
}
