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
	MenuRequestHandler* createMenuRequestHandler(LoggedUser user) noexcept;
	RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser user, Room room) noexcept;
	RoomMemberRequestHandler* createRoomMemberRequestHandler(LoggedUser user, Room room) noexcept;
	GameRequestHandler* createGameRequestHandler(LoggedUser user, Game& room) noexcept;
}; // namespace RequestHandlerFactory
