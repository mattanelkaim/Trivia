#include "Infrastructure/Server.h"
#include "Infrastructure/WSAInitializer.h"
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
		std::cerr << e.what() << '\n';
	}

	return 0;
}
