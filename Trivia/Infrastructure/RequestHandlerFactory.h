#pragma once

#include "IDatabase.h"
#include "LoginManager.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "MenuRequestHandler.h"
#include "LoggedUser.h"
#include <memory>
#include <mutex>

class LoginRequestHandler; // Double-circular-jerk-dependency-linkage mega-shit

class RequestHandlerFactory final
{
public:
    // methods
    MenuRequestHandler* createMenuRequestHandler(const LoggedUser& loggedUser);
    LoginRequestHandler* createLoginRequestHandler();

    // getters
    LoginManager* getLoginManager() noexcept;
    StatisticsManager* getStatisticsManager() noexcept;
    RoomManager* getRoomManager() noexcept;

    // Singleton
    RequestHandlerFactory() = delete;
    RequestHandlerFactory(RequestHandlerFactory& other) = delete;
    void operator=(const RequestHandlerFactory& other) = delete;
    static std::shared_ptr<RequestHandlerFactory> getInstance(IDatabase* db);
    ~RequestHandlerFactory() = default;

private:
    IDatabase* m_database;
    std::shared_ptr<LoginManager> m_loginManager;
    std::shared_ptr<RoomManager> m_roomManager;
    std::shared_ptr<StatisticsManager> m_statisticsManager;

    // Singleton
    explicit RequestHandlerFactory(IDatabase* db);
    inline static std::shared_ptr<RequestHandlerFactory> m_HandlerFactory = nullptr;
    inline static std::mutex m_mutex;
};
