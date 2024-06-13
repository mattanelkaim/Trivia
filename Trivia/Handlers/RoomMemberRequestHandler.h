#pragma once

#include "IRoomRequestHandler.h"
#include "LoggedUser.h"
#include "ServerDefinitions.h"

class RoomMemberRequestHandler : public IRoomRequestHandler
{
public:
	RoomMemberRequestHandler(IDatabase* db, LoggedUser user, Room room);

	bool isRequestRelevant(const RequestInfo& requestInfo) const noexcept override;
	RequestResult handleRequest(const RequestInfo& requestInfo) noexcept override;

private:
	RequestResult leaveRoom() noexcept;	
	RequestResult getRoomState() const noexcept;
};