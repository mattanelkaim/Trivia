#include "../Handlers/LoginRequestHandler.h"
#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory()
	: m_database(nullptr) {}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(*this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return this->m_loginManager;
}
