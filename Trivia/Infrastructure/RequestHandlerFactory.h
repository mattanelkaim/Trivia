#pragma once


#include "../Managers/LoginManager.h"
#include "IDatabase.h"

class LoginRequestHandler; // Double-circular-jerk-dependency-linkage mega-shit

class RequestHandlerFactory
{
private:
    IDatabase* m_database;
    LoginManager m_loginManager;

public:
    RequestHandlerFactory();
    LoginRequestHandler* createLoginRequestHandler();
    LoginManager& getLoginManager();
};
