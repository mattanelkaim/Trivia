#pragma once

#include "IRoomRequestHandler.h"
#include "LoggedUser.h"
#include "Room.h"
#include "ServerDefinitions.h"
#include "memory"

class RoomAdminRequestHandler final : public IRoomRequestHandler
{
public:
	RoomAdminRequestHandler(LoggedUser user, std::unique_ptr<Room>& room);
	~RoomAdminRequestHandler() noexcept override;

	bool isRequestRelevant(const RequestInfo& requestInfo) const noexcept override;
	RequestResult handleRequest(const RequestInfo& requestInfo) noexcept override;

	/*######################################
	#### AVOID SHITTY COMPILER WARNINGS ####
	######################################*/

	RoomAdminRequestHandler operator=(const RoomAdminRequestHandler& other) = delete;

private:	
	mutable bool m_hasExitedSafely;	
	RequestResult startRoomRequest() noexcept;
	RequestResult closeRoomRequest() noexcept;
};
