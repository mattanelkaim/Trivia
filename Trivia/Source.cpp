#include "Server.h"
#include "ServerDefinitions.h"
#include "WSAInitializer.h"
#include <iostream>
#include "../Infrastructure/SqliteDatabase.h"
int main(void)
{
	try
	{			
		WSAInitializer wsaInit;
		Server::getInstance()->run();
	}
	catch (const std::exception& e)
	{
		std::cerr << ANSI_RED << e.what() << ANSI_NORMAL << '\n';
	}

	return 0;
}
