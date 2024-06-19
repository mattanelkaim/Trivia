#include "Game.h"
#include "GameManager.h"
#include "NotFoundException.h"
#include "Question.h"
#include "Room.h"
#include "RoomManager.h"
#include "SqliteDatabase.h"
#include <cstdint>
#include <string>
#include <utility> // std::move
#include <vector>


const Game& GameManager::createGame(const Room& room)
{
    // Get questions from DB (not const to move later)
    std::vector<Question> questions = SqliteDatabase::getInstance().getQuestions(room.getData().numOfQuestionsInGame);
    
    if (questions.size() != room.getData().numOfQuestionsInGame)
        throw NotFoundException();

    return this->m_games.emplace_back(room.getData(), room.getAllUsers(), std::move(questions));
}

void GameManager::deleteGame(const uint32_t gameId) noexcept
{
    // Erase game from vector if it exists
    try
    {
       RoomManager::getInstance().deleteRoom(gameId);
       this->m_games.erase(this->findGame(gameId));
       // SubmitStatsToDB is called automatically on Game's destructor
    }
    catch (const NotFoundException&)
    {} // Ignore
}

std::vector<Game>::const_iterator GameManager::findGame(const uint32_t gameId) const
{
    for (auto it = m_games.cbegin(); it != m_games.cend(); ++it)
    {
        if (it->getGameData().id == gameId)
            return it;
    }
    throw NotFoundException("Game with id " + std::to_string(gameId));
}


// Singleton
GameManager& GameManager::getInstance() noexcept
{
    static GameManager instance; // This is thread-safe in C++11 and later
    return instance;
}
