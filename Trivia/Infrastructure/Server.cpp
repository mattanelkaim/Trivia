#include "Server.h"
#include <thread>

constexpr std::string_view EXIT = "EXIT";

void Server::run()
{
	std::string userInput;

	std::thread t_connector(&Communicator::startHandleRequests, &(this->m_communicator));
	t_connector.detach();

	do
	{
		std::cin >> userInput;
	} while (userInput != EXIT);
}
