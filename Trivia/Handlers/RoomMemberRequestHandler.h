#pragma once

#include "IDatabase.h"
#include "IRoomRequestHandler.h"
#include "LoggedUser.h"
#include "ServerDefinitions.h"

class RoomMemberRequestHandler final : public IRoomRequestHandler
{
public:
	RoomMemberRequestHandler(IDatabase* db, LoggedUser user, Room room);

	bool isRequestRelevant(const RequestInfo& requestInfo) const noexcept override;
	RequestResult handleRequest(const RequestInfo& requestInfo) noexcept override;

	/*######################################
	#### AVOID SHITTY COMPILER WARNINGS ####
	######################################*/

	RoomMemberRequestHandler operator=(const RoomMemberRequestHandler& other) = delete;

private:
	RequestResult leaveRoom() noexcept;	
	RequestResult getRoomState() const noexcept;
};