#pragma once

#include "ServerException.h"
#include <WinSock2.h>

class UnexpectedClientExit final : public ServerException
{
public:
    explicit UnexpectedClientExit(SOCKET crashedClient);
    const char* what() const noexcept override;
};
