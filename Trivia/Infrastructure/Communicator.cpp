#include "Communicator.h"
#include <iostream>
#include <WinSock2.h>

constexpr auto PORT = 7777;

Communicator::Communicator()
{}

void Communicator::bindAndListen() const
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(PORT); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY; // when there are few IPs for the machine. We will use always "INADDR_ANY"

	// Connects between the socket and the configuration (port and etc..)
	if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");

	std::cout << "Listening on port " << PORT << '\n'; // All good
}

int main(void)
{
	Communicator c;
	c.bindAndListen();

	return 0;
}
