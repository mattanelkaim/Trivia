#pragma warning(disable: 4061) // enumerator in switch of enum is not explicitly handled by a case label

#include "IRoomRequestHandler.h"
#include "JsonResponseSerializer.h"
#include "LoggedUser.h"
#include "RequestHandlerFactory.h"
#include "Room.h"
#include "RoomAdminRequestHandler.h"
#include "RoomManager.h"
#include "ServerDefinitions.h"
#include "ServerException.h"
#include <cstdint>
#include <utility> // std::move
#if SERVER_DEBUG
#include <iostream>
#endif

RoomAdminRequestHandler::RoomAdminRequestHandler(LoggedUser user, Room& room) :
    IRoomRequestHandler(std::move(user), room),
    m_hasExitedSafely(false)
{}

RoomAdminRequestHandler::~RoomAdminRequestHandler() noexcept
{
    if (!this->m_hasExitedSafely)
        this->closeRoomRequest();
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const noexcept
{
    switch (requestInfo.id)
    {
        case START_ROOM:
        case CLOSE_ROOM:
            return true;
        default:
            return IRoomRequestHandler::isRequestRelevant(requestInfo); // GET_ROOM_STATE
    }
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo) noexcept
{
    try
    {
        switch (requestInfo.id)
        {
        case GET_ROOM_STATE:
            return this->getRoomState();
            //break;

        case START_ROOM:
            return this->startRoomRequest();
            //break;

        case CLOSE_ROOM:
            return this->closeRoomRequest();
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

RequestResult RoomAdminRequestHandler::getRoomState() const noexcept
{
    return RequestResult{
        .response = this->getSerializedRoomState(), // IRoomRequestHandler
        .newHandler = nullptr // Keep the handler
    };
}

RequestResult RoomAdminRequestHandler::startRoomRequest() noexcept
{
    this->m_hasExitedSafely = true;

    this->m_room.updateRoomState(RoomStatus::CLOSED);

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(StartRoomResponse{OK}),
        .newHandler = nullptr // Keep the handler
    };
}

RequestResult RoomAdminRequestHandler::closeRoomRequest() const noexcept
{
    this->m_hasExitedSafely = true;

    const uint32_t roomId = this->m_room.getData().id;

    // Remove all users from the room
    for (const LoggedUser& user : this->m_room.getAllUsers())
    {
        // m_room better be in the vector of RoomManager, otherwise it will crash
        RoomManager::getInstance().getRoom(roomId).removeUser(user); // Removing each user from the room
    }

    // Delete the room
    RoomManager::getInstance().deleteRoom(roomId);

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(CloseRoomResponse{OK}),
        .newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user) // Return back to menu
    };
}
