#pragma once

#include "Game.h"
#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "ServerDefinitions.h"
#include <map>

class GameRequestHandler final : public IRequestHandler
{
public:
    /*######################################
    ############# CONSTRUCTORS #############
    ######################################*/

    GameRequestHandler() = delete;
    GameRequestHandler(const LoggedUser& user, Game& game) noexcept;

    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    bool isRequestRelevant(const RequestInfo& info) const noexcept override;
    RequestResult handleRequest(const RequestInfo& info) noexcept override;

    /*######################################
    #### AVOID SHITTY COMPILER WARNINGS ####
    ######################################*/
    GameRequestHandler operator=(const GameRequestHandler& other) = delete;

private:
    /*######################################
    ########### PRIVATE METHODS ############
    ######################################*/

    RequestResult getQuestion() noexcept;
    RequestResult getGameResults() const noexcept;
    RequestResult leaveGame() const noexcept;

    // @throws InvalidProtocolStructure
    RequestResult submitAnswer(const RequestInfo& info);

    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    Game& m_game;
    LoggedUser m_user;
    std::map<LoggedUser, GameData>::iterator m_playerIt; // Iterator in the Game's map
};
