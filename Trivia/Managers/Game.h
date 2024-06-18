#pragma once

#include "LoggedUser.h"
#include "Question.h"
#include "ServerDefinitions.h"
#include <cstdint>
#include <map>
#include <string>
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

    Question getQuestionForUser(const LoggedUser& user) const noexcept;
    bool submitAnswer(const LoggedUser& user, uint32_t answerId) noexcept; // TODO(mattan) UML is stoopit so validate this shit
    void removePlayer(const LoggedUser& user) noexcept;
    std::vector<PlayerResults> getGameResult() const noexcept;
    uint32_t getGameID() const noexcept;

private:
    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    void submitStatsToDB(const GameData& data);

    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    std::vector<Question> m_questions;
    std::map<LoggedUser, GameData> m_players;
    uint32_t m_gameId;
};
