#pragma once

#include "IRoomRequestHandler.h"
#include "Room.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"
#include "LoggedUser.h"
#include "IDatabase.h"
#include "ServerDefinitions.h"
#include "JsonResponseSerializer.h"
#include <type_traits> // std::move

IRoomRequestHandler::IRoomRequestHandler(IDatabase* db, LoggedUser user, Room room) :
    m_room(std::move(room)),
    m_user(std::move(user)),
    m_roomManager(RoomManager::getInstance()),
    m_handlerFactory(RequestHandlerFactory::getInstance(db))
{}

bool IRoomRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const noexcept
{
    return requestInfo.id == GET_ROOM_STATE;
}


buffer IRoomRequestHandler::getSerializedRoomState() const noexcept
{
    const RoomData& room = this->m_room.getData();

    return JsonResponseSerializer::serializeResponse(GetRoomStateResponse
    {
        .status = OK,
        .state = room.status,
        .hasGameBegun = (room.status == RoomStatus::CLOSED),
        .players = m_room.getAllUsers(),
        .questionCount = room.numOfQuestionsInGame,
        .answerTimeout = room.timePerQuestion,
    });
}
