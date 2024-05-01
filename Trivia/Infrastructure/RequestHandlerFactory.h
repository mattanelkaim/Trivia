#pragma once

#include "../Managers/LoginManager.h"
#include "IDatabase.h"
#include <mutex>

class LoginRequestHandler; // Double-circular-jerk-dependency-linkage mega-shit

class RequestHandlerFactory
{
public:
    LoginRequestHandler* createLoginRequestHandler();
    LoginManager* getLoginManager();

    // Singleton
    RequestHandlerFactory() = delete;
    RequestHandlerFactory(RequestHandlerFactory& other) = delete;
    void operator=(const RequestHandlerFactory& other) = delete;
    static RequestHandlerFactory* getInstance(IDatabase* db);

private:
    IDatabase* m_database;
    LoginManager* m_loginManager;

    // Singleton
    explicit RequestHandlerFactory(IDatabase* db);
    ~RequestHandlerFactory() = default;
    inline static RequestHandlerFactory* m_HandlerFactory = nullptr;
    static std::mutex m_mutex;
};
