#include "IDatabase.h"
#include "IRoomRequestHandler.h"
#include "JsonResponseSerializer.h"
#include "LoggedUser.h"
#include "RequestHandlerFactory.h"
#include "Room.h"
#include "RoomMemberRequestHandler.h"
#include "ServerDefinitions.h"
#include "ServerException.h"
#include <utility> // std::move
#if SERVER_DEBUG
#include <iostream>
#endif

RoomMemberRequestHandler::RoomMemberRequestHandler(IDatabase* db, LoggedUser user, Room room) :
    IRoomRequestHandler(db, std::move(user), std::move(room))
{}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const noexcept
{
    return this->IRoomRequestHandler::isRequestRelevant(requestInfo) || requestInfo.id == RequestId::LEAVE_ROOM;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo) noexcept
{
    try
    {
        switch (requestInfo.id)
        {
            case RequestId::LEAVE_ROOM:
                return this->leaveRoom();
                //break;

            case RequestId::GET_ROOM_STATE:
                return this->getRoomState();
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

RequestResult RoomMemberRequestHandler::leaveRoom() noexcept
{
    this->m_room.removeUser(this->m_user);

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(LeaveRoomResponse{OK}),
        .newHandler = this->m_handlerFactory->createMenuRequestHandler(m_user) // return back to menu
    };
}

RequestResult RoomMemberRequestHandler::getRoomState() const noexcept
{
    return RequestResult{
        .response = this->getSerializedRoomState(),
        .newHandler = this->m_handlerFactory->createRoomMemberRequestHandler(m_user, m_room)
    };
}
