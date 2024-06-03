#pragma warning(disable: 4061) // enumerator in switch of enum is not explicitly handled by a case label

#include "IDatabase.h"
#include "InvalidProtocolStructure.h"
#include "JsonRequestDeserializer.hpp"
#include "JsonResponseSerializer.h"
#include "LoggedUser.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "RoomManager.h"
#include "ServerDefinitions.h"
#include "ServerException.h"
#include "StatisticsManager.h"
#include <cstdint>
#include <utility> // std::move
#if SERVER_DEBUG
#include <iostream>
#endif


MenuRequestHandler::MenuRequestHandler(IDatabase* db, LoggedUser user) :
    m_handlerFactory(RequestHandlerFactory::getInstance(db)),
    m_roomManager(RoomManager::getInstance()),
    m_statisticsManager(StatisticsManager::getInstance(db)),
    m_user(std::move(user))
{}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& info) const noexcept
{
    return info.id == GET_PLAYERS_IN_ROOM || info.id == JOIN_ROOM || \
           info.id == CREATE_ROOM         || info.id == GET_ROOMS || \
           info.id == GET_STATISTICS      || info.id == LOGOUT;
}

// NOLINTNEXTLINE(bugprone-exception-escape) - ignore std::bad_alloc
RequestResult MenuRequestHandler::handleRequest(const RequestInfo& info) noexcept
{
    try
    {
        switch (info.id)
        {
        case LOGOUT:
            return this->logout();
            //break;

        case GET_STATISTICS:
            return this->getPersonalStats();
            //break;

        case GET_HIGHSCORE:
            return this->getHighScore();
            //break;

        case GET_ROOMS:
            return this->getRooms();
            //break;

        case GET_PLAYERS_IN_ROOM:
            return this->getPlayersInRoom(info);
            //break;

        case CREATE_ROOM:
            return this->createRoom(info);
            //break;

        case JOIN_ROOM:
            return this->joinRoom(info);
            //break;

        default:
            throw InvalidProtocolStructure("Request is not relevant to MenuRequestHandler!");
            //break;
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

RequestResult MenuRequestHandler::logout() const noexcept
{
    this->m_handlerFactory->getLoginManager()->logout(this->m_user);

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(LogoutResponse{OK}),
        .newHandler = nullptr
    };
}

RequestResult MenuRequestHandler::getPersonalStats() const
{
    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(GetPlayersInRoomResponse{this->m_statisticsManager->getUserStatistics(m_user)}),
        .newHandler = this->m_handlerFactory->createMenuRequestHandler(m_user)
    };
}

RequestResult MenuRequestHandler::getHighScore() const
{
    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(GetHighScoreResponse{OK, this->m_statisticsManager->getHighScore()}),
        .newHandler = this->m_handlerFactory->createMenuRequestHandler(m_user)
    };
}

RequestResult MenuRequestHandler::getRooms() const noexcept
{
    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(GetRoomsResponse{OK, this->m_roomManager->getRooms()}),
        .newHandler = this->m_handlerFactory->createMenuRequestHandler(m_user)
    };
}

RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo& info) const
{
    const uint32_t roomId = JsonRequestDeserializer::deserializeRequest<GetPlayersInRoomRequest>(info.buffer).roomId;

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(GetPlayersInRoomResponse{this->m_roomManager->getRoom(roomId).getAllUsers()}),
        .newHandler = this->m_handlerFactory->createMenuRequestHandler(m_user)
    };
}

RequestResult MenuRequestHandler::createRoom(const RequestInfo& info) const
{
    const auto request = JsonRequestDeserializer::deserializeRequest<CreateRoomRequest>(info.buffer);

    // Creating a room as specified in the request buffer
    RoomManager::getInstance()->createRoom(m_user, {
        .name = request.roomName,
        .id = RoomManager::getNextRoomId(),
        .maxPlayers = request.maxUsers,
        .numOfQuestionsInGame = request.questionCount,
        .timePerQuestion = request.answerTimeout,
        .status = RoomStatus::OPEN
    });

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(CreateRoomResponse{OK}),
        .newHandler = this->m_handlerFactory->createMenuRequestHandler(m_user)
    };
}

RequestResult MenuRequestHandler::joinRoom(const RequestInfo& info) const
{
    const uint32_t roomId = JsonRequestDeserializer::deserializeRequest<JoinRoomRequest>(info.buffer).roomId;

    // Adding the user to the room specified in the request buffer
    this->m_roomManager->getRoom(roomId).addUser(m_user);

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(JoinRoomResponse{OK}),
        .newHandler = this->m_handlerFactory->createMenuRequestHandler(m_user)
    };
}
