#include "UnexpectedClientExit.h"

UnexpectedClientExit::UnexpectedClientExit(const SOCKET crashedClient) :
	IServerException("Client socket " + std::to_string(crashedClient) + " unexpectedly disconnected")
{}

const char* UnexpectedClientExit::what() const
{
	return this->m_err.c_str();
}
