#pragma once

#include "Game.h"
#include "Room.h"
#include <cstdint>
#include <vector>

class GameManager final
{
public:
    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    /**
    * Creates a new room from the previous (waiting) room.
    * @param room The (waiting) room from which the game started.
    * @return A reference of created game.
    * @throws InvalidSQL If there is an exception with the DB.
    * @throws NotFoundException if cannot fetch questions from DB.
    */
    Game& createGame(const Room& room);

    void deleteGame(uint32_t gameId) noexcept;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    GameManager(const GameManager& other) = delete;
    void operator=(const GameManager& other) = delete;
    static GameManager& getInstance() noexcept;
    ~GameManager() noexcept = default;

private:
    /*######################################
    ############ PRIVATE METHODS ###########
    ######################################*/

    /**
    * Searches for a game in the manager's game vector by its ID.
    * @param gameId The ID of the game to search for.
    * @return A const iterator of the found game within the vector.
    * @throws NotFoundException If no game with the specified ID is found.
    */
    std::vector<Game>::const_iterator findGame(uint32_t gameId) const;

    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    std::vector<Game> m_games;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    GameManager() noexcept = default;
};
