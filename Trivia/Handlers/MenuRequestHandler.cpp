#include "JsonResponseSerializer.h"
#include "MenuRequestHandler.h"
#include "ServerDefinitions.h"


bool MenuRequestHandler::isRequestRelevant(const RequestInfo& info) const noexcept
{
    return info.id != LOGIN && info.id != SIGNUP && info.id;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& info)
{
    switch (info.id)
    {
        case GET_PLAYERS_IN_ROOM:
            return this->getPlayersInRoom(info);
            //break;

        case JOIN_ROOM:
            return this->joinRoom(info);
            //break;
            
        case CREATE_ROOM:
            return this->createRoom(info);
            //break;

        case GET_ROOMS:
            return this->getRooms(info);
            //break;

        case GET_STATISTICS:
            return this->getPersonalStats(info);
            //break;

        case LOGOUT:
            return this->logout(info);
            //break;  

        default:
            return RequestResult{ .response = JsonResponseSerializer::serializeResponse(ErrorResponse{"Not supported yet"}),
                                  .newHandler = nullptr };
            //break;
    }
}

RequestResult MenuRequestHandler::logout(const RequestInfo& info)
{
    return RequestResult{ .response = JsonResponseSerializer::serializeResponse(LogoutResponse{RESPONSE}),
                          .newHandler = nullptr };
}

RequestResult MenuRequestHandler::getRooms(const RequestInfo& info)
{
    return RequestResult{ .response = JsonResponseSerializer::serializeResponse(GetRoomsResponse{RESPONSE}),
                          .newHandler = };
}


