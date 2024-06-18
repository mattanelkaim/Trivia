#include "Game.h"
#include "GameRequestHandler.h"
#include "LoggedUser.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "Room.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include <utility> // std::move


MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& user)
{
    return new MenuRequestHandler(user);
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser& user, Room room)
{
    return new RoomAdminRequestHandler(user, std::move(room));
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser& user, Room room)
{
    return new RoomMemberRequestHandler(user, std::move(room));
}

GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(const LoggedUser& user, Game& room)
{
    return new GameRequestHandler(user, std::move(room));
}
