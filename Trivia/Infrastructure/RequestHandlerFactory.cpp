#include "Game.h"
#include "GameRequestHandler.h"
#include "LoggedUser.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "SafeRoom.h"
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

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser user, safe_room& room) noexcept
{
    return new RoomAdminRequestHandler(std::move(user), room);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser user, safe_room& room) noexcept
{
    return new RoomMemberRequestHandler(std::move(user), room);
}
GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(LoggedUser user, Game& game) noexcept
{
    return new GameRequestHandler(std::move(user), game);
}
//NOLINTEND(bugprone-unhandled-exception-at-new)
#pragma warning(pop)
