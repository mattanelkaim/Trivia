#pragma warning(disable: 4061) // enumerator in switch of enum is not explicitly handled by a case label

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


RoomMemberRequestHandler::RoomMemberRequestHandler(LoggedUser user, Room& room) :
    IRoomRequestHandler(std::move(user), room)
{}

RoomMemberRequestHandler::~RoomMemberRequestHandler() noexcept
{
    this->m_room.removeUser(this->m_user);
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const noexcept
{
    return requestInfo.id == LEAVE_ROOM || \
        this->IRoomRequestHandler::isRequestRelevant(requestInfo); // GET_ROOM_STATE
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo) noexcept
{
    try
    {
        switch (requestInfo.id)
        {
            case GET_ROOM_STATE:
                return this->getRoomState();
                //break;

            case LEAVE_ROOM:
                return this->leaveRoom();
                //break;

            default: // This should not happen
                throw ServerException("Request is not relevant to MenuRequestHandler!");
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

RequestResult RoomMemberRequestHandler::getRoomState() const noexcept
{
    return RequestResult{
        .response = this->getSerializedRoomState(), // IRoomRequestHandler
        .newHandler = nullptr // Keep the handler
    };
}

RequestResult RoomMemberRequestHandler::leaveRoom() noexcept
{
    this->m_room.removeUser(this->m_user);

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(LeaveRoomResponse{OK}),
        .newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user) // Return back to menu
    };
}
