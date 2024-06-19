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
    Game(RoomData roomData, const std::vector<LoggedUser>& users) noexcept;

    ~Game() noexcept;

    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    std::optional<Question> getQuestionForUser(const LoggedUser& user) noexcept;
    std::vector<PlayerResults> getGameResult() const noexcept;
    const RoomData& getGameData() const noexcept;
    
    // @throws NotFoundException
    void removePlayer(const LoggedUser& user);

    // @throws NotFoundException if user not found
    uint8_t submitAnswer(const LoggedUser& user, uint8_t answerId);

private:
    /*######################################
    ############ PRIVATE METHODS ###########
    ######################################*/

    // @throws InvalidSQL
    static void submitStatsToDB(const LoggedUser& user, const GameData& data);

    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    RoomData m_data;
    std::vector<Question> m_questions;
    std::map<LoggedUser, GameData> m_players;
};
