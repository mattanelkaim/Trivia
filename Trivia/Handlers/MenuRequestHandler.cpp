#pragma warning(disable: 4061) // enumerator in switch of enum is not explicitly handled by a case label

#include "JsonRequestDeserializer.hpp"
#include "JsonResponseSerializer.h"
#include "LoggedUser.h"
#include "LoginManager.h"
#include "MenuRequestHandler.h"
#include "NotFoundException.h"
#include "RequestHandlerFactory.h"
#include "Room.h"
#include "RoomManager.h"
#include "ServerDefinitions.h"
#include "ServerException.h"
#include "StatisticsManager.h"
#include <cstdint>
#include <optional>
#include <utility> // std::move
#if SERVER_DEBUG
#include "Helper.h"
#include <iostream>
#endif


MenuRequestHandler::MenuRequestHandler(LoggedUser user) noexcept :
    m_user(std::move(user))
{}

MenuRequestHandler::~MenuRequestHandler() noexcept
{
    this->logout();
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& info) const noexcept
{
    switch (info.id)
    {
        case GET_PLAYERS_IN_ROOM:
        case JOIN_ROOM:
        case CREATE_ROOM:
        case GET_ROOMS:
        case GET_STATISTICS:
        case LOGOUT:
        case GET_HIGHSCORE:
            return true;
        default:
            return false;
    }
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

        default: // This should not happen
            throw ServerException("Request is not relevant to MenuRequestHandler!");
        }
    }
    catch (const ServerException& e)
    {
        if constexpr (SERVER_DEBUG)
            std::cerr << ANSI_RED << Helper::formatError(__FUNCTION__, e.what()) << ANSI_NORMAL << '\n';

        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(ErrorResponse{"Invalid protocol structure"}),
            .newHandler = nullptr
        };
    }
}

RequestResult MenuRequestHandler::logout() const noexcept
{
    LoginManager::getInstance().logout(this->m_user);

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(LogoutResponse{OK}),
        .newHandler = nullptr
    };
}

RequestResult MenuRequestHandler::getPersonalStats() const
{
    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(GetPersonalStatsResponse{{OK}, StatisticsManager::getUserStatistics(m_user)}),
        .newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user)
    };
}

RequestResult MenuRequestHandler::getHighScore() const
{
    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(GetHighScoreResponse{{OK}, StatisticsManager::getHighScore()}),
        .newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user)
    };
}

RequestResult MenuRequestHandler::getRooms() const noexcept
{
    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(GetRoomsResponse{{OK}, RoomManager::getInstance().getRooms()}),
        .newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user)
    };
}

RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo& info) const
{
    const uint32_t roomId = JsonRequestDeserializer::deserializeRequest<GetPlayersInRoomRequest>(info.buffer).roomId;

    try
    {
        const Room& room = RoomManager::getInstance().getRoom(roomId);
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(GetPlayersInRoomResponse{{OK}, room.getAllUsers()}),
            .newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user)
        };
    }
    catch (const NotFoundException&) // thrown by getRoom()
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(GetPlayersInRoomResponse{{ERR_NOT_FOUND}, {}}), // Empty vector
            .newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user)
        };
    }
}

RequestResult MenuRequestHandler::createRoom(const RequestInfo& info) const
{
    const auto request = JsonRequestDeserializer::deserializeRequest<CreateRoomRequest>(info.buffer);

    if (RoomManager::getInstance().doesRoomExist(request.roomName))
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(CreateRoomResponse{ERR_ALREADY_EXISTS}),
            .newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user)
        };
    }

    // Creating a room as specified in the request buffer
    std::optional<Room> createdRoom = RoomManager::getInstance().createRoom(m_user, RoomData{
        .name = request.roomName,
        .id = RoomManager::getNextRoomId(), // Generate a unique ID
        .maxPlayers = request.maxUsers,
        .numOfQuestionsInGame = request.questionCount,
        .timePerQuestion = request.answerTimeout,
        .status = RoomStatus::OPEN
    });

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(CreateRoomResponse{OK}),
        .newHandler = RequestHandlerFactory::createRoomAdminRequestHandler(m_user, createdRoom.value())
    };
}

RequestResult MenuRequestHandler::joinRoom(const RequestInfo& info) const
{
    const uint32_t roomId = JsonRequestDeserializer::deserializeRequest<JoinRoomRequest>(info.buffer).roomId;

    ResponseCode responseCode{};
    IRequestHandler* newHandler = nullptr;
    try
    {
        Room& room = RoomManager::getInstance().getRoom(roomId);

        // Adding the user to the room specified in the request buffer
        responseCode = room.addUser(m_user);

        if (responseCode == OK)
            newHandler = RequestHandlerFactory::createRoomMemberRequestHandler(m_user, room);
        else
            newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user); // Retry joining
    }
    catch (const NotFoundException&) // thrown by getRoom()
    {
        responseCode = ERR_NOT_FOUND;
        newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user);
    }

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(JoinRoomResponse{responseCode}),
        .newHandler = newHandler
    };
}
