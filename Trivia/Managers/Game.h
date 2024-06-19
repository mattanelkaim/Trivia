#pragma once

#include "LoggedUser.h"
#include "Question.h"
#include "ServerDefinitions.h"
#include <cstdint>
#include <ctime>
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
    Game(RoomData roomData, const std::vector<LoggedUser>& users, std::vector<Question> questions) noexcept;

    ~Game() noexcept;

    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    std::optional<Question> getQuestionForUser(const LoggedUser& user) noexcept;
    // @throws NotFoundException if user not found
    uint8_t submitAnswer(const LoggedUser& user, uint8_t answerId);
    // @throws NotFoundException
    void removePlayer(const LoggedUser& user);
    std::vector<PlayerResults> getGameResult() const noexcept;
    const RoomData& getGameData() const noexcept;
    std::map<LoggedUser, GameData>::iterator getPlayerIt(const LoggedUser& user) noexcept;

    
    friend class GameRequestHandler;
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
    time_t m_timeGameStarted;
};
