#pragma warning(disable: 4061) // enumerator in switch of enum is not explicitly handled by a case label

#include "Game.h"
#include "GameRequestHandler.h"
#include "JsonResponseSerializer.h"
#include "LoggedUser.h"
#include "ServerDefinitions.h"
#include "ServerException.h"
#include <utility> // std::move
#if SERVER_DEBUG
#include "Helper.h"
#include <iostream>
#endif


GameRequestHandler::GameRequestHandler(const LoggedUser user, Game& game) noexcept :
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
        switch (info.id)
        {
        //case GET_QUESTION:
        //    return this->getQuestion(info);
        //    //break;

        //case SUBMIT_ANSWER:
        //    return this->submitAnswer(info);
        //    //break;

        //case GET_GAME_RESULTS:
        //    return this->getGameResults(info);
        //    //break;

        //case LEAVE_GAME:
        //    return this->leaveGame(info);
        //    //break;

        default: // This should not happen
            throw ServerException("Request is not relevant to GameRequestHandler!");
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
