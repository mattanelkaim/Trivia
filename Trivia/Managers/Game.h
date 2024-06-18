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

    Game() noexcept = default;

    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    Question getQuestionForUser(const LoggedUser& user) const noexcept;
    void submitAnswer(const LoggedUser& user, const std::string& answer) noexcept; // TODO(mattan) UML is stoopit so validate this shit
    void removePlayer(const LoggedUser& user) noexcept;

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
