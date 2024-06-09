#include "WSAInitializer.h"
#include <stdexcept>
#include <string>
#include <WinSock2.h>
#include <Windows.h>

WSAInitializer::WSAInitializer()
{
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
        throw std::runtime_error("WSAStartup Failed: error " + std::to_string(WSAGetLastError()));
}

WSAInitializer::~WSAInitializer() noexcept
{
    WSACleanup();
}
