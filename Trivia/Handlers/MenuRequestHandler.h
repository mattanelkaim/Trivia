#pragma once

#include "IDatabase.h"
#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "ServerDefinitions.h"
#include <RoomManager.h>
#include <StatisticsManager.h>

class RequestHandlerFactory; // Double-circular-jerk-dependency-linkage mega-shit

class MenuRequestHandler final : public IRequestHandler
{
public:
    explicit MenuRequestHandler(IDatabase* db, LoggedUser user);

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

    RequestHandlerFactory* m_handlerFactory;
    RoomManager* m_roomManager;
    StatisticsManager* m_statisticsManager;
    const LoggedUser m_user;

    /*######################################
    ############ HELPER METHODS ############
    ######################################*/

    RequestResult logout() const noexcept;

    // @throws InvalidSQL
    RequestResult getPersonalStats() const;

    // @throws InvalidSQL
    RequestResult getHighScore() const;

    RequestResult getRooms() const noexcept;

    /**
    * @throws ServerException If the room does not exist.
    * @throws InvalidProtocolStructure
    */
    RequestResult getPlayersInRoom(const RequestInfo& info) const;

    /**
    * @throws ServerException If the room already exists.
    * @throws InvalidProtocolStructure
    */
    RequestResult createRoom(const RequestInfo& info) const;

    /**
    * @throws ServerException If the room does not exist.
    * @throws InvalidProtocolStructure
    */
    RequestResult joinRoom(const RequestInfo& info) const;
};
