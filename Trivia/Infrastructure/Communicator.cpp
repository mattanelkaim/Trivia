#include "Communicator.h"
#include "Helper.h"
#include <iostream>
#include <string>
#include <thread>
#include <WinSock2.h>

using std::to_string;
constexpr unsigned short PORT = 7777;
constexpr auto EXIT = "EXIT";

Communicator::Communicator()
{
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (this->m_serverSocket == INVALID_SOCKET)
		throw std::runtime_error(__FUNCTION__ " - socket() err: " + to_string(WSAGetLastError()));
}

Communicator::~Communicator() noexcept
{
	for (const auto& client : this->m_clients)
		delete client.second;
}

void Communicator::startHandleRequests()
{
	this->bindAndListen();
}

void Communicator::bindAndListen()
{
	struct sockaddr_in serverAddr{};
	serverAddr.sin_family = AF_INET; // Must be AF_INET
	serverAddr.sin_port = htons(PORT); // Port that server will listen for
	serverAddr.sin_addr.s_addr = INADDR_ANY; // When there are few IPs for the machine. We will use always "INADDR_ANY"

	// Connects the socket and the configuration
	if (bind(this->m_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		throw std::runtime_error(__FUNCTION__ " - bind() err: " + to_string(WSAGetLastError()));

	// Start listening for incoming client requests
	if (listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::runtime_error(__FUNCTION__ " - listen() err: " + to_string(WSAGetLastError()));

	std::cout << "Listening on port " << PORT << "...\n";
	std::thread connector(&Communicator::serverListen, this);
	connector.detach();
}

void Communicator::serverListen()
{
	do
	{
		std::cout << "Waiting for new client connection request\n";
		acceptClient();
	} while (true);
}

void Communicator::acceptClient()
{
	// This accepts the client and create a specific socket from server to this client
	// The process will not continue until a client connects to the server
	SOCKET client_socket = accept(m_serverSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
		throw std::runtime_error(__FUNCTION__ " - accept() err: " + to_string(WSAGetLastError()));

	std::cout << "Client accepted. Server and client can communicate\n";

	// the function that handle the conversation with the client
	std::thread handlerThread(&Communicator::handleNewClient, this, client_socket);
	handlerThread.detach();
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	const std::string msg = Helper::getStringPartFromSocket(clientSocket, 5);

	// "hello" just for now
	if (msg == "hello")
		this->m_clients.emplace(clientSocket, new LoginRequestHandler);

	std::cout << "client sent " << msg << ". Echoing back...\n";
	Helper::sendData(clientSocket, msg);
}


#include "WSAInitializer.h"
int main(void)
{
	try
	{
		WSAInitializer wsaInit;
		Communicator c;
		c.startHandleRequests();
		
		std::string userInput;
		do
		{
			std::cin >> userInput;
		} while (userInput != EXIT);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
	}

	return 0;
}
