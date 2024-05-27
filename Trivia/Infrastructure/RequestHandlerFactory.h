#pragma once

#include "IDatabase.h"
#include "LoggedUser.h"
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include <memory>
#include <mutex>

class RequestHandlerFactory final
{
public:
    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    MenuRequestHandler* createMenuRequestHandler(const LoggedUser& user);
    LoginRequestHandler* createLoginRequestHandler();

    /*######################################
    ################ GETTERS ###############
    ######################################*/

    LoginManager* getLoginManager() noexcept;
    StatisticsManager* getStatisticsManager() noexcept;
    RoomManager* getRoomManager() noexcept;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    RequestHandlerFactory() = delete;
    RequestHandlerFactory(const RequestHandlerFactory& other) = delete;
    void operator=(const RequestHandlerFactory& other) = delete;
    static RequestHandlerFactory* getInstance(IDatabase* db);
    ~RequestHandlerFactory() = default;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    IDatabase* m_database;
    LoginManager* m_loginManager;
    RoomManager* m_roomManager;
    StatisticsManager* m_statisticsManager;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    explicit RequestHandlerFactory(IDatabase* db);
    inline static std::unique_ptr<RequestHandlerFactory> m_HandlerFactory = nullptr;
    inline static std::mutex m_mutex;
};
