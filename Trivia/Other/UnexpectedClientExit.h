#pragma once

#include "IServerException.h"
#include <WinSock2.h>

class UnexpectedClientExit : public IServerException
{
public:
	explicit UnexpectedClientExit(SOCKET crashedClient);
	const char* what() const override;
};

