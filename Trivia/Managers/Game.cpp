#include "Game.h"
#include "InvalidSQL.h"
#include "LoggedUser.h"
#include "NotFoundException.h"
#include "Question.h"
#include "RoomManager.h"
#include "ServerDefinitions.h"
#include "SqliteDatabase.h"
#include <cstdint>
#include <Helper.h>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <vector>


Game::Game(const uint32_t roomId) noexcept :
    m_gameId(roomId)
{}

Game::~Game() noexcept
{
    try
    {
        for (const auto& [user, data] : this->m_players)
        {
            Game::submitStatsToDB(user, data);
        }
    }
    catch (const InvalidSQL& e)
    {
        std::cerr << ANSI_RED << Helper::formatError(__FUNCTION__, std::string("Cannot submit stats to DB: ") + e.what()) << ANSI_NORMAL << '\n';
    }
}

uint8_t Game::submitAnswer(const LoggedUser& user, uint8_t answerId)
{
    try
    {
        const uint8_t correctAnswerId = this->m_players.at(user).currentQuestion->getCorrectAnswerId();

        if (answerId == correctAnswerId)
            this->m_players.at(user).correctAnswerCount++;
        else
            this->m_players.at(user).wrongAnswerCount++;

        return correctAnswerId;
    }
    catch (const std::out_of_range&)
    {
        throw NotFoundException("User '" + user + "' in game!");
    }
}

void Game::submitStatsToDB(const LoggedUser& user, const GameData& data)
{
    // Functions is simply a bridge to SqliteDatabase
    SqliteDatabase::getInstance().updatePlayerStats(user, data);
}

void Game::removePlayer(const LoggedUser& user)
{
    RoomManager::getInstance().getRoom(this->m_gameId).removeUser(user);
    this->m_players.erase(user);
}

std::optional<Question> Game::getQuestionForUser(const LoggedUser& user) noexcept
{
    try
    {
        if (++(this->m_players.at(user).currentQuestion) == m_questions.cend()) [[unlikely]]
        {
            return std::nullopt;
        }

        return *(this->m_players.at(user).currentQuestion); // Dereference the iterator
    }
    catch (const std::out_of_range&) // If user does not exist for some reason
    {
        return std::nullopt;
    }
}

// NOLINTNEXTLINE(bugprone-exception-escape) - ignore std::bad_alloc
std::vector<PlayerResults> Game::getGameResult() const noexcept
{
    std::vector<PlayerResults> results;
    results.reserve(this->m_players.size());

    for (const auto& [user, data] : this->m_players)
    {
        results.emplace_back(user, data.correctAnswerCount, data.wrongAnswerCount, data.averageAnswerTime);
    }

    return results;
}

uint32_t Game::getGameID() const noexcept
{
    return this->m_gameId;
}
