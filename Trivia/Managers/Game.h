#pragma once

#include "LoggedUser.h"
#include "Question.h"
#include "ServerDefinitions.h"
#include <cstdint>
#include <map>
#include <optional>
#include <vector>

class Game final
{
public:
    /*######################################
    ############# CONSTRUCTORS #############
    ######################################*/

    // ID is identical to the room ID
    Game() = delete;
    explicit Game(uint32_t roomId) noexcept;

    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    std::optional<Question> getQuestionForUser(const LoggedUser& user) noexcept;
    std::vector<PlayerResults> getGameResult() const noexcept;
    uint32_t getGameID() const noexcept;
    
    // @throws NotFoundException
    void removePlayer(const LoggedUser& user);

    // @throws NotFoundException if user not found
    uint8_t submitAnswer(const LoggedUser& user, uint8_t answerId);

private:
    /*######################################
    ############ PRIVATE METHODS ###########
    ######################################*/

    void submitStatsToDB(const GameData& data);

    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    std::vector<Question> m_questions;
    std::map<LoggedUser, GameData> m_players;
    uint32_t m_gameId;
};
