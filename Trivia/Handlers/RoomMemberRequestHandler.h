#pragma once

#include "IRoomRequestHandler.h"
#include "LoggedUser.h"
#include "Room.h"
#include "ServerDefinitions.h"

class RoomMemberRequestHandler final : public IRoomRequestHandler
{
public:
	RoomMemberRequestHandler(LoggedUser user, std::unique_ptr<Room>& room);
	~RoomMemberRequestHandler() noexcept override;

	bool isRequestRelevant(const RequestInfo& requestInfo) const noexcept override;
	RequestResult handleRequest(const RequestInfo& requestInfo) noexcept override;

	/*######################################
	#### AVOID SHITTY COMPILER WARNINGS ####
	######################################*/

	RoomMemberRequestHandler operator=(const RoomMemberRequestHandler& other) = delete;

private:
	RequestResult leaveRoom() noexcept;	
	RequestResult getRoomState() noexcept;
};