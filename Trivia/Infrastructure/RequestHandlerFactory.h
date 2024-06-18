#pragma once

#include "Game.h"
#include "GameRequestHandler.h"
#include "LoggedUser.h"
#include "MenuRequestHandler.h"
#include "Room.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

namespace RequestHandlerFactory
{
	MenuRequestHandler* createMenuRequestHandler(LoggedUser user);
	RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser user, Room room);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(LoggedUser user, Room room);
	GameRequestHandler* createGameRequestHandler(LoggedUser user, Game& room);
}; // namespace RequestHandlerFactory
