#include "Server.h"
#include "./WSAInitializer.h"
#include <iostream>

constexpr std::string_view EXIT = "EXIT";

void Server::run()
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
}
