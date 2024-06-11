#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(IDatabase* db, const LoggedUser user, Room room) :
    m_handlerFactory(RequestHandlerFactory::getInstance(db)),
    m_roomManager(RoomManager::getInstance()),
    m_user(std::move(user)),
    m_room(std::move(room))
{}