#pragma warning(disable: 4061) // enumerator in switch of enum is not explicitly handled by a case label

#include "Game.h"
#include "GameRequestHandler.h"
#include "Helper.h"
#include "JsonResponseSerializer.h"
#include "LoggedUser.h"
#include "ServerDefinitions.h"
#include "ServerException.h"
#include <utility> // std::move
#if SERVER_DEBUG
#include <iostream>
#endif


GameRequestHandler::GameRequestHandler(LoggedUser user, Game& game) noexcept :
    m_game(game),
    m_user(std::move(user))
{}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& info) const noexcept
{
    switch (info.id)
    {
    case SUBMIT_ANSWER:
    case LEAVE_GAME:
    case GET_QUESTION:
    case GET_GAME_RESULTS:
        return true;
    default:
        return false;
    }
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo& info) noexcept
{
    try
    {

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
