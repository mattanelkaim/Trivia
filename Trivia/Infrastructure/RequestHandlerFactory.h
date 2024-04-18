#pragma once

#include "../Handlers/LoginRequestHandler.h"
#include "../Managers/LoginManager.h"
#include "IDatabase.h"

class RequestHandlerFactory
{
private:
	IDatabase* m_database;
	LoginManager m_loginManager;

public:
	LoginRequesetHandler createLoginRequestHandler();
	LoginManager& getLoginManager();
};
