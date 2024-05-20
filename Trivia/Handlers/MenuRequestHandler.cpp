#include "JsonResponseSerializer.h"
#include "MenuRequestHandler.h"
#include "ServerDefinitions.h"


bool MenuRequestHandler::isRequestRelevant(const RequestInfo& info) const noexcept
{
    return info.id != LOGIN && info.id != SIGNUP && info.id != LOGOUT;
}

RequestResult MenuRequestHandler::handleRequest([[maybe_unused]] const RequestInfo& info)
{
    if (!this->isRequestRelevant(info))
        return RequestResult{.response = JsonResponseSerializer::serializeResponse(ErrorResponse{"Not supported yet"}),
                             .newHandler = nullptr };

    // if the request is relevant
    switch (info.id)
    {
    case GET_PLAYERS_IN_ROOM:
        return this->getPlayersInRoom(info);
        break;

    case JOIN_ROOM:
        return this->joinRoom(info);
        break;
            
    case CREATE_ROOM:
        return this->createRoom(info);
        break;

    case GET_ROOMS:
        return this->getRooms(info);
        break;

    case GET_STATISTICS:
        return this->getPersonalStats(info);
        break;

    case SIGNOUT:
        return this->signout(info);
        break;       
    }
}


