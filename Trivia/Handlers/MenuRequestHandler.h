#pragma once

#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "ServerDefinitions.h"

class MenuRequestHandler final : public IRequestHandler
{
public:
    explicit MenuRequestHandler(LoggedUser user);

    // Logs out the user
    ~MenuRequestHandler() override;

    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    bool isRequestRelevant(const RequestInfo& info) const noexcept override;
    RequestResult handleRequest(const RequestInfo& info) noexcept override;

    /*######################################
    #### AVOID SHITTY COMPILER WARNINGS ####
    ######################################*/

    MenuRequestHandler operator=(const MenuRequestHandler& other) = delete;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    LoggedUser m_user;

    /*######################################
    ############ HELPER METHODS ############
    ######################################*/

    RequestResult logout() const noexcept;

    // @throws InvalidSQL
    RequestResult getPersonalStats() const;

    // @throws InvalidSQL
    static RequestResult getHighScore();

    static RequestResult getRooms() noexcept;

    // @throws InvalidProtocolStructure
    static RequestResult getPlayersInRoom(const RequestInfo& info);

    // @throws InvalidProtocolStructure
    RequestResult createRoom(const RequestInfo& info) const;

    // @throws InvalidProtocolStructure
    RequestResult joinRoom(const RequestInfo& info) const;
};
