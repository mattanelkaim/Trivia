#pragma once

#include "IRequestHandler.h"
#include "Room.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"
#include "LoggedUser.h"
#include "IDatabase.h"

class RequestHandlerFactory; // Double-circular-jerk-dependency-linkage mega-shit

class RoomAdminRequestHandler : public IRequestHandler
{
public:
	explicit RoomAdminRequestHandler(IDatabase* db, const LoggedUser user, Room room);

private:
	Room m_room;
	const LoggedUser m_user;
	RoomManager* m_roomManager;
	RequestHandlerFactory* m_handlerFactory;
};