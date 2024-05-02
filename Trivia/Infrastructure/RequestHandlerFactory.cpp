#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"

// Singleton
RequestHandlerFactory* m_HandlerFactory = nullptr;
std::mutex RequestHandlerFactory::m_mutex;


RequestHandlerFactory* RequestHandlerFactory::getInstance(IDatabase* db)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_HandlerFactory == nullptr)
    {
        m_HandlerFactory = new RequestHandlerFactory(db);
    }
    return m_HandlerFactory;
}

RequestHandlerFactory::RequestHandlerFactory(IDatabase* db)
    : m_database(db), m_loginManager(LoginManager::getInstance(db)) {}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(this);
}

LoginManager* RequestHandlerFactory::getLoginManager()
{
    return this->m_loginManager;
}
