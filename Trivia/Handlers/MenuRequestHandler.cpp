#include "JsonResponseSerializer.h"
#include "JsonRequestDeserializer.hpp"
#include "MenuRequestHandler.h"
#include "ServerDefinitions.h"


MenuRequestHandler::MenuRequestHandler(IDatabase* db, const LoggedUser& user) :
    m_handlerFactory(RequestHandlerFactory::getInstance(db)),
    m_roomManager(RoomManager::getInstance()),
    m_statisticsManager(StatisticsManager::getInstance(db)),
    m_user(user)
{}

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

RequestResult MenuRequestHandler::logout([[maybe_unused]] const RequestInfo& info) noexcept
{    
    return RequestResult{ .response = JsonResponseSerializer::serializeResponse(LogoutResponse{RESPONSE}),
                          .newHandler = nullptr };
}

RequestResult MenuRequestHandler::getRooms([[maybe_unused]] const RequestInfo& info) noexcept
{
    return RequestResult{ .response = JsonResponseSerializer::serializeResponse(GetRoomsResponse{RESPONSE, this->m_roomManager->getRooms()}),
                          .newHandler = this->m_handlerFactory->createMenuRequestHandler(m_user)};
}

RequestResult MenuRequestHandler::getPlayersInRoom([[maybe_unused]] const RequestInfo& info) noexcept
{    
    return RequestResult{ .response = JsonResponseSerializer::serializeResponse(GetPlayersInRoomResponse{this->m_roomManager->getRoom(JsonRequestDeserializer::deserializeRequest<GetPlayersInRoomRequest>(info.buffer).roomId).getAllUsers()}),
                          .newHandler = this->m_handlerFactory->createMenuRequestHandler(m_user) };
}

RequestResult MenuRequestHandler::getPersonalStats([[maybe_unused]] const RequestInfo& info) noexcept
{
    return RequestResult{ .response = JsonResponseSerializer::serializeResponse(GetPlayersInRoomResponse{this->m_statisticsManager->getUserStatistics(m_user)}),
                          .newHandler = this->m_handlerFactory->createMenuRequestHandler(m_user) };
}

RequestResult MenuRequestHandler::getHighScore([[maybe_unused]] const RequestInfo& info) noexcept
{
    return RequestResult{ .response = JsonResponseSerializer::serializeResponse(GetHighScoreResponse{RESPONSE, this->m_statisticsManager->getHighScore()}),
                          .newHandler = this->m_handlerFactory->createMenuRequestHandler(m_user) };
}

RequestResult MenuRequestHandler::joinRoom([[maybe_unused]] const RequestInfo& info) noexcept
{
    // adding the user to the room specified in the request buffer
    RoomManager::getInstance()->getRoom(JsonRequestDeserializer::deserializeRequest<JoinRoomRequest>(info.buffer).roomId).addUser(m_user);

    return RequestResult{ .response = JsonResponseSerializer::serializeResponse(JoinRoomResponse{RESPONSE}),
                          .newHandler = this->m_handlerFactory->createMenuRequestHandler(m_user)};
}

RequestResult MenuRequestHandler::createRoom([[maybe_unused]] const RequestInfo& info) noexcept
{
    CreateRoomRequest request = JsonRequestDeserializer::deserializeRequest<CreateRoomRequest>(info.buffer);
    // creating a room as specifies in the request buffer
    RoomManager::getInstance()->createRoom(m_user, { .name = request.roomName,
                                                     .id = RoomManager::getNextRoomId(),
                                                     .maxPlayers = (unsigned short)request.maxUsers,
                                                     .numOfQuestionsInGame = (unsigned short)request.questionCount,
                                                     .timePerQuestion = request.answerTimeout,
                                                     .status = 1
                                                   });
    return RequestResult{ .response = JsonResponseSerializer::serializeResponse(CreateRoomResponse{RESPONSE}),
                          .newHandler = this->m_handlerFactory->createMenuRequestHandler(m_user) };
}


