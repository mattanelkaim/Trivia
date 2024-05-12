/*
Things to do before shipping
- Build with compiler optimizations (O2/O3)
- Turn off ASan
*/

#include "Server.h"
#include "ServerDefinitions.h"
#include "WSAInitializer.h"
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
        std::cerr << ANSI_RED << e.what() << ANSI_NORMAL << '\n';
    }

    return 0;
}
