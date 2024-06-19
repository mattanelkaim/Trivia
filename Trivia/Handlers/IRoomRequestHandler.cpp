#include "IRoomRequestHandler.h"
#include "JsonResponseSerializer.h"
#include "LoggedUser.h"
#include "SafeRoom.h"
#include "ServerDefinitions.h"
#include <utility> // std::move


IRoomRequestHandler::IRoomRequestHandler(LoggedUser user, safe_room& room) noexcept :
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

    // Create non-const response to move later
    GetRoomStateResponse response = GetRoomStateResponse
    { // Cannot use designators cuz status isn't explicitly named in GetRoomStateResponse
        /*.status =*/ {ResponseCode::OK},
        /*.state =*/ room.status,
        /*.hasGameBegun =*/ (room.status == RoomStatus::CLOSED),
        /*.players =*/ m_room.room.getAllUsers(),
        /*.questionCount =*/ room.numOfQuestionsInGame,
        /*.answerTimeout =*/ room.timePerQuestion
    };

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(std::move(response)),
        .newHandler = nullptr
    };
}
