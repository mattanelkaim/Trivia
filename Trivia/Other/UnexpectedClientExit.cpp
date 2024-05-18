#include "ServerException.h"
#include "UnexpectedClientExit.h"
#include <string>

UnexpectedClientExit::UnexpectedClientExit(const SOCKET crashedClient) :
    ServerException("Client socket " + std::to_string(crashedClient) + " unexpectedly disconnected")
{}

const char* UnexpectedClientExit::what() const noexcept
{
    return this->m_err.c_str();
}
