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
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility> // std::move
#include <vector>


Game::Game(RoomData roomData, const std::vector<LoggedUser>& users, std::vector<Question> questions) noexcept :
    m_data(std::move(roomData)),
    m_questions(std::move(questions))
{
    const GameData defaultGameData{
        .currentQuestion = this->m_questions.cbegin(),
        .correctAnswerCount = 0,
        .wrongAnswerCount = 0,
        .totalAnswerTime = 0
    };

    for (const LoggedUser& user : users)
        this->m_players.emplace(user, defaultGameData);
}

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

uint8_t Game::submitAnswer(const LoggedUser& user, const uint8_t answerId)
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
    RoomManager::getInstance().getRoom(this->m_data.id).removeUser(user);
    this->m_players.erase(user);
}

std::optional<Question> Game::getQuestionForUser(const LoggedUser& user) noexcept
{
    try
    {
        // Get next question if there are any left
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
        results.emplace_back(user, data.correctAnswerCount, data.wrongAnswerCount, data.totalAnswerTime);
    }

    return results;
}

const RoomData& Game::getGameData() const noexcept
{
    return this->m_data;
}

std::map<LoggedUser, GameData>::iterator Game::getPlayerIt(const LoggedUser& user) noexcept
{
    return this->m_players.find(user);
}
