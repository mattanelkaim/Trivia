#pragma once

#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "Room.h"
#include "ServerDefinitions.h"
#include <memory>

class IRoomRequestHandler : public IRequestHandler
{
public:
	IRoomRequestHandler(LoggedUser user, std::unique_ptr<Room>& room);

	inline bool isRequestRelevant(const RequestInfo& requestInfo) const noexcept override;
	RequestResult handleRequest(const RequestInfo& info) override = 0;

	/*######################################
	#### AVOID SHITTY COMPILER WARNINGS ####
	######################################*/

	IRoomRequestHandler operator=(const IRoomRequestHandler& other) = delete;

protected:
	virtual RequestResult getRoomState() noexcept;

	std::unique_ptr<Room>& m_room;
	const LoggedUser m_user;
};
