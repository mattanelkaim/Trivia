#pragma once

#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "Room.h"
#include "ServerDefinitions.h"

class IRoomRequestHandler : public IRequestHandler
{
public:
	IRoomRequestHandler(LoggedUser user, Room room) noexcept;

	bool isRequestRelevant(const RequestInfo& requestInfo) const noexcept override;
	RequestResult handleRequest(const RequestInfo& info) noexcept override = 0;

	/*######################################
	#### AVOID SHITTY COMPILER WARNINGS ####
	######################################*/

	IRoomRequestHandler operator=(const IRoomRequestHandler& other) = delete;

protected:
	buffer getSerializedRoomState() const noexcept;

	Room m_room;
	const LoggedUser m_user;
};
