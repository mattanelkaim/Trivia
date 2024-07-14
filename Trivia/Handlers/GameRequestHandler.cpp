#pragma warning(disable: 4061) // enumerator in switch of enum is not explicitly handled by a case label

#include "Game.h"
#include "GameRequestHandler.h"
#include "JsonRequestDeserializer.hpp"
#include "JsonResponseSerializer.h"
#include "LoggedUser.h"
#include "NotFoundException.h"
#include "Question.h"
#include "RequestHandlerFactory.h"
#include "ServerDefinitions.h"
#include "ServerException.h"
#include <cstdint>
#include <ctime>
#include <map>
#include <optional>
#include <string>
#include <utility> // std::move
#if SERVER_DEBUG
#include "Helper.h"
#endif


GameRequestHandler::GameRequestHandler(const LoggedUser& user, Game& game) noexcept :
    m_game(game),
    m_user(user),
    m_playerIt(game.getPlayerIt(user))
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
        case GET_QUESTION:
            return this->getQuestion();
            //break;

        case SUBMIT_ANSWER:
            return this->submitAnswer(info);
            //break;

        case GET_GAME_RESULTS:
            return this->getGameResults();
            //break;

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
            Helper::safePrintError(Helper::formatError(__FUNCTION__, e.what()));

        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(ErrorResponse{"Invalid protocol structure"}),
            .newHandler = nullptr
        };
    }
}

RequestResult GameRequestHandler::getQuestion() noexcept
{
    // Time that game is running / time per question = totalQuestionsAsked (index)
    const auto currentQuestionInGame = ((std::time(nullptr) - m_game.m_timeGameStarted) / m_game.m_data.timePerQuestion) + 1;

    const uint32_t totalAnswered = m_playerIt->second.correctAnswerCount + m_playerIt->second.wrongAnswerCount;

    // Check if user is ahead of the game
    if (totalAnswered >= static_cast<uint32_t>(currentQuestionInGame))
    {
        return RequestResult{
            // Send an empty question and an empty list of answers
            .response = JsonResponseSerializer::serializeResponse(GetQuestionResponse{{WAIT_FOR_OTHERS}, "", {}}),
            .newHandler = nullptr // Stay in the game
        };
    }

    // Try to send a question to the user
    const std::optional<Question> question = this->m_game.getQuestionForUser(this->m_user);

    // If no more questions, return an empty question response
    if (!question.has_value()) [[unlikely]]
    {
        return RequestResult{
            // Send an empty question and an empty list of answers
            .response = JsonResponseSerializer::serializeResponse(GetQuestionResponse{{NO_MORE_QUESTIONS}, "", {}}),
            .newHandler = nullptr // Stay in the game
        };
    }

    // Else extract the possible answers and send the question info
    std::map<uint32_t, std::string> possibleAnswers;
    for (uint32_t i = 0; const std::string& answer : question.value().getPossibleAnswers())
    {
        possibleAnswers[++i] = answer;
    }

    // Register time when client got question
    m_playerIt->second.gotQuestionTime = std::time(nullptr);

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(GetQuestionResponse{{OK}, question.value().getQuestion(), possibleAnswers}),
        .newHandler = nullptr // Stay in the game
    };
}

RequestResult GameRequestHandler::submitAnswer(const RequestInfo& info)
{
    // Add answerTime of this question to the total
    const time_t answerTime = std::time(nullptr) - m_playerIt->second.gotQuestionTime;
    m_playerIt->second.totalAnswerTime += static_cast<uint32_t>(answerTime);

    const auto request = JsonRequestDeserializer::deserializeRequest<SubmitAnswerRequest>(info.buffer);

    const uint8_t correctAnsId = this->m_game.submitAnswer(this->m_user, request.answerId);

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(SubmitAnswerResponse{{OK}, correctAnsId}),
        .newHandler = nullptr // Stay in the game
    };
}

RequestResult GameRequestHandler::getGameResults() const noexcept
{
    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(GetGameResultsResponse{{OK}, this->m_game.getGameResult()}),
        .newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user) // Return back to menu
    };
}

RequestResult GameRequestHandler::leaveGame() const noexcept
{
    // Try to leave the game if room exists
    try
    {
        this->m_game.removePlayer(this->m_user);
    }
    catch (const NotFoundException&)
    {} // Do nothing

    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(LeaveGameResponse{OK}),
        .newHandler = RequestHandlerFactory::createMenuRequestHandler(m_user) // Return back to menu
    };
}
