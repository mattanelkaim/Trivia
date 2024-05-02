#pragma once

#include "IDatabase.h"
#include "LoginManager.h"

class LoginRequestHandler; // Double-circular-jerk-dependency-linkage mega-shit

class RequestHandlerFactory
{
private:
    IDatabase* m_database;
    LoginManager m_loginManager;

public:
    explicit RequestHandlerFactory(IDatabase* db);
    RequestHandlerFactory() = delete;

    LoginRequestHandler* createLoginRequestHandler();
    LoginManager& getLoginManager();
};
