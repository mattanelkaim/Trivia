#pragma once

#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "Room.h"
#include "ServerDefinitions.h"

class RequestHandlerFactory; // Double-circular-jerk-dependency-linkage mega-shit

class IRoomRequestHandler : public IRequestHandler
{
public:
	IRoomRequestHandler(LoggedUser user, Room room);

	inline bool isRequestRelevant(const RequestInfo& requestInfo) const noexcept override;
	RequestResult handleRequest(const RequestInfo& info) override = 0;

	/*######################################
	#### AVOID SHITTY COMPILER WARNINGS ####
	######################################*/

	IRoomRequestHandler operator=(const IRoomRequestHandler& other) = delete;

protected:
	buffer getSerializedRoomState() const noexcept;

	Room m_room;
	const LoggedUser m_user;
};
