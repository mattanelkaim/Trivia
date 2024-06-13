#pragma once

#include "IDatabase.h"
#include "IRoomRequestHandler.h"
#include "LoggedUser.h"
#include "Room.h"

class RoomAdminRequestHandler final : public IRoomRequestHandler
{
public:
	RoomAdminRequestHandler(IDatabase* db, LoggedUser user, Room room);

	bool isRequestRelevant(const RequestInfo& requestInfo) const noexcept override;
	RequestResult handleRequest(const RequestInfo& requestInfo) noexcept override;

	/*######################################
	#### AVOID SHITTY COMPILER WARNINGS ####
	######################################*/

	RoomAdminRequestHandler operator=(const RoomAdminRequestHandler& other) = delete;

private:	
	RequestResult getRoomState() const noexcept;
	RequestResult startRoomRequest() const noexcept;
	RequestResult closeRoomRequest() const noexcept;
};