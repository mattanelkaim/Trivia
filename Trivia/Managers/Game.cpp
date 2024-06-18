#include "Game.h"
#include "ServerDefinitions.h"
#include <cstdint>
#include <vector>


Game::Game(uint32_t roomId) noexcept :
    m_gameId(roomId)
{}

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
