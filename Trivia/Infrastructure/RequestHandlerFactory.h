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
    static std::shared_ptr<RequestHandlerFactory> getInstance(IDatabase* db);
    ~RequestHandlerFactory() = default;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    IDatabase* m_database;
    std::shared_ptr<LoginManager> m_loginManager;
    std::shared_ptr<RoomManager> m_roomManager;
    std::shared_ptr<StatisticsManager> m_statisticsManager;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    explicit RequestHandlerFactory(IDatabase* db);
    inline static std::shared_ptr<RequestHandlerFactory> m_HandlerFactory = nullptr;
    inline static std::mutex m_mutex;
};
