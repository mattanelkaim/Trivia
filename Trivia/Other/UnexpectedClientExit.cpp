#include "ServerException.h"
#include "UnexpectedClientExit.h"
#include <string>

UnexpectedClientExit::UnexpectedClientExit(const SOCKET crashedClient) :
    ServerException("Client socket " + std::to_string(crashedClient) + " unexpectedly disconnected")
{}
