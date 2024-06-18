#include "Game.h"
#include "LoggedUser.h"
#include "NotFoundException.h"
#include "RoomManager.h"
#include "ServerDefinitions.h"
#include <cstdint>
#include <stdexcept>
#include <vector>


Game::Game(const uint32_t roomId) noexcept :
    m_gameId(roomId)
{}

uint8_t Game::submitAnswer(const LoggedUser& user, uint8_t answerId)
{
    try
    {
        const uint8_t correctAnswerId = this->m_players.at(user).currentQuestion.getCorrectAnswerId();

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

void Game::removePlayer(const LoggedUser& user)
{
    RoomManager::getInstance().getRoom(this->m_gameId).removeUser(user);
    this->m_players.erase(user);
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
