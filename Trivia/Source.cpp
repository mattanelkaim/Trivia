#include "Infrastructure/Server.h"
#include "Infrastructure/WSAInitializer.h"
#include <iostream>

constexpr std::string_view EXIT = "EXIT";

int main(void)
{
	try
	{
		WSAInitializer wsaInit;
		Server server;
		server.run();

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
