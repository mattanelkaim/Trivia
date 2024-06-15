#pragma once

#include "IRoomRequestHandler.h"
#include "JsonResponseSerializer.h"
#include "LoggedUser.h"
#include "Room.h"
#include "ServerDefinitions.h"
#include <utility> // std::move


IRoomRequestHandler::IRoomRequestHandler(LoggedUser user, Room room) :
    m_room(std::move(room)),
    m_user(std::move(user))
{}

bool IRoomRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const noexcept
{
    return requestInfo.id == GET_ROOM_STATE;
}

buffer IRoomRequestHandler::getSerializedRoomState() const noexcept
{
    const RoomData& room = this->m_room.getData();

    return JsonResponseSerializer::serializeResponse(GetRoomStateResponse
    { // Cannot use designators cuz status isn't explicitly named in GetRoomStateResponse
        /*.status =*/ {ResponseCode::OK},
        /*.state =*/ room.status,
        /*.hasGameBegun =*/ (room.status == RoomStatus::CLOSED),
        /*.players =*/ m_room.getAllUsers(),
        /*.questionCount =*/ room.numOfQuestionsInGame,
        /*.answerTimeout =*/ room.timePerQuestion
    });
}
