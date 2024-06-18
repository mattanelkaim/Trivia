#include "Game.h"

Game::Game(uint32_t roomId) noexcept :
    m_gameId(roomId)
{}

uint32_t Game::getGameID() const noexcept
{
    return this->m_gameId;
}
