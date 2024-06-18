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

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser& user, std::unique_ptr<Room>& room)
{
    return new RoomAdminRequestHandler(user, room);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser& user, std::unique_ptr<Room>& room)
{
    return new RoomMemberRequestHandler(user, room);
}
