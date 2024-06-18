#include "Game.h"
#include "GameRequestHandler.h"
#include "LoggedUser.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "Room.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include <utility> // std::move


MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user)
{
    return new MenuRequestHandler(std::move(user));
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser user, Room room)
{
    return new RoomAdminRequestHandler(std::move(user), std::move(room));
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser user, Room room)
{
    return new RoomMemberRequestHandler(std::move(user), std::move(room));
}

GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(LoggedUser user, Game& room)
{
    return new GameRequestHandler(std::move(user), room);
}
