#include "Game.h"
#include "GameRequestHandler.h"
#include "LoggedUser.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "Room.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include <utility> // std::move


#pragma warning(push)
#pragma warning(disable: 26447) // Shut up the allocation exception shit
//NOLINTBEGIN(bugprone-unhandled-exception-at-new)
MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user) noexcept
{
    return new MenuRequestHandler(std::move(user));
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser user, Room room) noexcept
{
    return new RoomAdminRequestHandler(std::move(user), std::move(room));
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser user, Room room) noexcept
{
    return new RoomMemberRequestHandler(std::move(user), std::move(room));
}

GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(LoggedUser user, Game& room) noexcept
{
    return new GameRequestHandler(std::move(user), room);
}
//NOLINTEND(bugprone-unhandled-exception-at-new)
#pragma warning(pop)
