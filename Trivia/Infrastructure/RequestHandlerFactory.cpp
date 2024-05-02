#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* db)
	: m_database(db), m_loginManager(LoginManager(db)) {}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(*this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return this->m_loginManager;
}
