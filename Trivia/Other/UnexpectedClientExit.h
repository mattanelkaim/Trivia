#pragma once

#include "ServerException.h"
#include <WinSock2.h>

class UnexpectedClientExit final : public ServerException
{
public:
    UnexpectedClientExit() = delete;
    explicit UnexpectedClientExit(SOCKET crashedClient);
};
