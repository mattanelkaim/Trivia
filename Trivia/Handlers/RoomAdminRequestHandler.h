#pragma once

#include "IRoomRequestHandler.h"
#include "Room.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"
#include "LoggedUser.h"
#include "IDatabase.h"

class RoomAdminRequestHandler : public IRoomRequestHandler
{
public:
	RoomAdminRequestHandler(IDatabase* db, LoggedUser user, Room room);

	bool isRequestRelevant(const RequestInfo& requestInfo) const noexcept override;
	RequestResult handleRequest(const RequestInfo& requestInfo) noexcept override;
private:	
	RequestResult getRoomState() const noexcept;
	RequestResult startRoomRequest() const noexcept;
	RequestResult closeRoomRequest() const noexcept;
};