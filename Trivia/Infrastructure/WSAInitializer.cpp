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
    // Q: why is this try necessary ?
    // A: to avoid throwing exceptions in d-tors !
    // if we do throw think what will happened in regular exception, our object
    // will be destroyed because an exception occurred and then the d-tor will be
    // called and if we throw now there is no one to handle the exception because
    // we are already in the flow of exception handling !! (inception...)
    // please read more about exception handling and why it's forbidden to throw
    // exception from the destructor.
    try
    {
        WSACleanup();
    }
    catch (...) {}
}
