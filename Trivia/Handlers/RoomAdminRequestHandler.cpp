#include "RoomAdminRequestHandler.h"
#include "Room.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"
#include "LoggedUser.h"
#include "IDatabase.h"
#include "ServerDefinitions.h"
#include "ServerException.h"
#include "JsonResponseSerializer.h"
#include <cstdint>
#if SERVER_DEBUG
#include <iostream>
#endif

RoomAdminRequestHandler::RoomAdminRequestHandler(IDatabase* db, LoggedUser user, Room room) :
    IRoomRequestHandler(db, std::move(user), std::move(room))
{}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const noexcept
{
    return this->IRoomRequestHandler::isRequestRelevant(requestInfo) || \
                             requestInfo.id == RequestId::CLOSE_ROOM || \
                             requestInfo.id == RequestId::START_ROOM;
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo) noexcept
{
    try
    {
        switch (requestInfo.id)
        {
        case RequestId::GET_ROOM_STATE:
            return this->getRoomState();
            //break;

        case RequestId::CLOSE_ROOM:
            return this->closeRoomRequest();
            //break;

        case RequestId::START_ROOM:
            return this->startRoomRequest();
            //break;

        // This should not happen
        default:
            throw ServerException("Request is not relevant to MenuRequestHandler!");
            //break;
        }
    }
    catch (const ServerException& e)
    {
        if constexpr (SERVER_DEBUG)
            std::cerr << ANSI_RED << e.what() << ANSI_NORMAL << '\n';

        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(ErrorResponse{"Invalid protocol structure"}),
            .newHandler = nullptr
        };
    }
}

RequestResult RoomAdminRequestHandler::closeRoomRequest() const noexcept
{
    const uint32_t roomId = this->m_room.getData().id;

    for (const LoggedUser& user : this->m_room.getAllUsers())
    {
        RoomManager::getInstance()->getRoom(roomId).removeUser(user); // removing each user from the room
    }
    RoomManager::getInstance()->deleteRoom(roomId); // deleting the room after all members have left

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(CloseRoomResponse{OK}),
        .newHandler = this->m_handlerFactory->createMenuRequestHandler(m_user) // return back to menu
    };
}

RequestResult RoomAdminRequestHandler::getRoomState() const noexcept
{
    return RequestResult{
        .response = this->getSerializedRoomState(),
        .newHandler = this->m_handlerFactory->createRoomAdminRequestHandler(m_user, m_room)
    };
}

RequestResult RoomAdminRequestHandler::startRoomRequest() const noexcept
{
    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(StartRoomResponse{OK}),
        .newHandler = this->m_handlerFactory->createRoomAdminRequestHandler(m_user, m_room) // return back to menu
    };
}
