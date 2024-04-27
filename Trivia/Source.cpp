#include "Infrastructure/Server.h"
#include "Infrastructure/WSAInitializer.h"
#include <iostream>

#include "ServerDefenitions.h"
int main(void)
{
	RequestInfo r;
	std::cout << __cplusplus << '\n';
	std::cout << sizeof(r.id) << '\n';
	std::cout << sizeof(r.receivalTime) << '\n';
	std::cout << sizeof(r.buffer) << '\n';
	std::cout << sizeof(r) << "\n\n";

	try
	{
		WSAInitializer wsaInit;
		Server server;
		server.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
	}

	return 0;
}
