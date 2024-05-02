#include "Server.h"
#include "WSAInitializer.h"
#include <iostream>

int main(void)
{
	try
	{		
		WSAInitializer wsaInit;
		Server server;
		server.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "\033[31;1m" << e.what() << "\033[0m\n";
	}

	return 0;
}
