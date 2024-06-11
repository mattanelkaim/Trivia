#pragma once

#include "IRequestHandler.h"
#include "Room.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"
#include "LoggedUser.h"
#include "IDatabase.h"

class RoomMemberRequestHandler : public IRequestHandler
{
public:
	explicit RoomMemberRequestHandler(IDatabase* db, const LoggedUser user, Room room);

private:
	Room m_room;
	const LoggedUser m_user;
	RoomManager* m_roomManager;
	RequestHandlerFactory* m_handlerFactory;
};