#include "Infrastructure/Server.h"
#include "Infrastructure/WSAInitializer.h"
#include <iostream>

int main(void)
{
	try
	{
		WSAInitializer wsaInit;
		Server::getInstance()->run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "\033[31;1m" << e.what() << "\033[0m\n";
	}

	return 0;
}
