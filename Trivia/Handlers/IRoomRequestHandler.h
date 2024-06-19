#pragma once

#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "SafeRoom.h"
#include "ServerDefinitions.h"

class IRoomRequestHandler : public IRequestHandler
{
public:
	IRoomRequestHandler(LoggedUser user, safe_room& room) noexcept;

	bool isRequestRelevant(const RequestInfo& requestInfo) const noexcept override;
	RequestResult handleRequest(const RequestInfo& info) noexcept override = 0;

	/*######################################
	#### AVOID SHITTY COMPILER WARNINGS ####
	######################################*/

	IRoomRequestHandler operator=(const IRoomRequestHandler& other) = delete;

protected:
	virtual RequestResult getRoomState() noexcept;

	safe_room& m_room;
	const LoggedUser m_user;
};
