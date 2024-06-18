#pragma once

#include "IRoomRequestHandler.h"
#include "JsonResponseSerializer.h"
#include "LoggedUser.h"
#include "../Infrastructure/SafeRoom.h"
#include "ServerDefinitions.h"
#include <utility> // std::move


IRoomRequestHandler::IRoomRequestHandler(LoggedUser user, safe_room& room) :
    m_room(room),
    m_user(std::move(user))
{}

bool IRoomRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const noexcept
{
    return requestInfo.id == GET_ROOM_STATE;
}

RequestResult IRoomRequestHandler::getRoomState() noexcept
{
    const RoomData& room = this->m_room.room.getData();

    return RequestResult{JsonResponseSerializer::serializeResponse(GetRoomStateResponse
                        { // Cannot use designators cuz status isn't explicitly named in GetRoomStateResponse
                            /*.status =*/ {ResponseCode::OK},
                            /*.state =*/ room.status,
                            /*.hasGameBegun =*/ (room.status == RoomStatus::CLOSED),
                            /*.players =*/ m_room.room.getAllUsers(),
                            /*.questionCount =*/ room.numOfQuestionsInGame,
                            /*.answerTimeout =*/ room.timePerQuestion
                        }),
                        nullptr };
}
