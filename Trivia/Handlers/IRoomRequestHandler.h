#pragma once

#include "IRequestHandler.h"
#include "Room.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"
#include "LoggedUser.h"
#include "IDatabase.h"
#include "ServerDefinitions.h"

class RequestHandlerFactory; // Double-circular-jerk-dependency-linkage mega-shit

class IRoomRequestHandler : public IRequestHandler
{
public:
	IRoomRequestHandler(IDatabase* db, LoggedUser user, Room room);

	virtual inline bool isRequestRelevant(const RequestInfo& requestInfo) const noexcept;
	RequestResult handleRequest(const RequestInfo& info) override = 0;

protected:
	buffer getSerializedRoomState() const noexcept;

	Room m_room;
	const LoggedUser m_user;
	RoomManager* m_roomManager;
	RequestHandlerFactory* m_handlerFactory;
};
