#include "Game.h"
#include "GameManager.h"
#include "NotFoundException.h"
#include "Room.h"
#include <cstdint>
#include <string>
#include <vector>


const Game& GameManager::createGame(const Room& room) noexcept
{
    return this->m_games.emplace_back(room.getRoomId(), room.getAllUsers());
}

void GameManager::deleteGame(uint32_t gameId) noexcept
{
    try
    {
        this->m_games.erase(this->findGame(gameId));
    }
    catch (const NotFoundException&)
    {} // Ignore
}

GameManager& GameManager::getInstance() noexcept
{
    static GameManager instance; // This is thread-safe in C++11 and later
    return instance;
}

std::vector<Game>::const_iterator GameManager::findGame(uint32_t gameId) const
{
    for (auto it = m_games.cbegin(); it != m_games.cend(); ++it)
    {
        if (it->getGameID() == gameId)
            return it;
    }
    throw NotFoundException("Game with id " + std::to_string(gameId));
}
