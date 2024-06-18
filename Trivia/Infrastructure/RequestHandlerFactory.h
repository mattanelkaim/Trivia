#pragma once

#include "LoggedUser.h"
#include "MenuRequestHandler.h"
#include "Room.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

namespace RequestHandlerFactory
{
	MenuRequestHandler* createMenuRequestHandler(const LoggedUser& user);
	RoomAdminRequestHandler* createRoomAdminRequestHandler(const LoggedUser& user, safe_room& room);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(const LoggedUser& user, safe_room& room);
}; // namespace RequestHandlerFactory
