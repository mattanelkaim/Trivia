#pragma once

#include "Game.h"
#include "GameRequestHandler.h"
#include "LoggedUser.h"
#include "MenuRequestHandler.h"
#include "SafeRoom.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

namespace RequestHandlerFactory
{
	MenuRequestHandler* createMenuRequestHandler(LoggedUser user) noexcept;
	RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser user, safe_room& room) noexcept;
	RoomMemberRequestHandler* createRoomMemberRequestHandler(LoggedUser user, safe_room& room) noexcept;
	GameRequestHandler* createGameRequestHandler(LoggedUser user, Game& game) noexcept;
}; // namespace RequestHandlerFactory
