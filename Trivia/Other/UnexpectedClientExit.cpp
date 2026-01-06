#include "ServerException.h"
#include "UnexpectedClientExit.h"
#include <string>
#include <WinSock2.h>

UnexpectedClientExit::UnexpectedClientExit(SOCKET crashedClient) :
    ServerException("Client socket " + std::to_string(crashedClient) + " unexpectedly disconnected")
{}
