#pragma once

#include "LoggedUser.h"
#include "MenuRequestHandler.h"
#include "Room.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

namespace RequestHandlerFactory
{
	MenuRequestHandler* createMenuRequestHandler(const LoggedUser& user);
	RoomAdminRequestHandler* createRoomAdminRequestHandler(const LoggedUser& user, Room& room);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(const LoggedUser& user, Room& room);
}; // namespace RequestHandlerFactory
