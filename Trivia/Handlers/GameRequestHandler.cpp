#pragma warning(disable: 4061) // enumerator in switch of enum is not explicitly handled by a case label

#include "Game.h"
#include "GameRequestHandler.h"
#include "JsonRequestDeserializer.hpp"
#include "JsonResponseSerializer.h"
#include "LoggedUser.h"
#include "NotFoundException.h"
#include "Question.h"
#include "RequestHandlerFactory.h"
#include "RoomManager.h"
#include "ServerDefinitions.h"
#include "ServerException.h"
#include <cstdint>
#include <map>
#include <string>
#include <utility> // std::move
#if SERVER_DEBUG
#include "Helper.h"
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

        case LEAVE_GAME:
            return this->leaveGame();
            //break;

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

RequestResult GameRequestHandler::getQuestion() noexcept
{
    const Question question = this->m_game.getQuestionForUser(this->m_user);

    std::map<uint32_t, std::string> possibleAnswers;
    for (uint32_t i = 0; const std::string& answer : question.getPossibleAnswers())
    {
        possibleAnswers[++i] = answer;
    }

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(GetQuestionResponse{{OK}, question.getQuestion(), possibleAnswers}),
        .newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user) // Return back to menu
    };
}

RequestResult GameRequestHandler::getGameResults() noexcept
{
    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(GetGameResultsResponse{{OK}, this->m_game.getGameResult()}),
        .newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user) // Return back to menu
    };
}

RequestResult GameRequestHandler::leaveGame() noexcept
{
    // Try to leave the game if room exists
    try
    {
        RoomManager::getInstance().getRoom(this->m_game.getGameID()).removeUser(this->m_user);
    }
    catch (const NotFoundException&)
    {} // Do nothing

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(LeaveGameResponse{OK}),
        .newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user) // Return back to menu
    };
}

RequestResult GameRequestHandler::submitAnswer(const RequestInfo& info)
{
    const auto request = JsonRequestDeserializer::deserializeRequest<SubmitAnswerRequest>(info.buffer);

    const uint32_t correctAnsId = this->m_game.submitAnswer(this->m_user, request.answerId);

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(SubmitAnswerResponse{{OK}, correctAnsId}),
        .newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user) // Return back to menu
    };
}
