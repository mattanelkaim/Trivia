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
#include "Helper.h"
#endif


RoomMemberRequestHandler::RoomMemberRequestHandler(LoggedUser user, safe_room& room) :
    IRoomRequestHandler(std::move(user), room),
    m_hasExitedSafely(false)
{}

RoomMemberRequestHandler::~RoomMemberRequestHandler() noexcept
{
    if (!this->m_hasExitedSafely)
        this->m_room.room.removeUser(this->m_user);
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const noexcept
{
    return requestInfo.id == LEAVE_ROOM || \
        this->IRoomRequestHandler::isRequestRelevant(requestInfo); // GET_ROOM_STATE
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo) noexcept
{
    if (this->wasRoomClosed())
    {
        this->m_hasExitedSafely = true;
        return RequestResult{ JsonResponseSerializer::serializeResponse(ErrorResponse{"Room was not found"}),
                              RequestHandlerFactory::createMenuRequestHandler(this->m_user) };
    }
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
                throw ServerException("Request is not relevant to RoomMemberRequestHandler!");
        }
    }
    catch (const ServerException& e)
    {
        if constexpr (SERVER_DEBUG)
            Helper::safePrintError(Helper::formatError(__FUNCTION__, e.what()));

        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(ErrorResponse{"Invalid protocol structure"}),
            .newHandler = nullptr
        };
    }
}

RequestResult RoomMemberRequestHandler::leaveRoom() noexcept
{
    this->m_room.room.removeUser(this->m_user);

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(LeaveRoomResponse{OK}),
        .newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user) // Return back to menu
    };
}

bool RoomMemberRequestHandler::wasRoomClosed() const noexcept
{
    if (m_room.doesRoomExist.load())
    {
        return false;
    }
    
    m_room.numThreadsInRoom.store(static_cast<uint16_t>(m_room.numThreadsInRoom.load() - 1));
    return true;
}
