#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"


RequestHandlerFactory::RequestHandlerFactory(IDatabase* db) :
    m_database(db),
    m_loginManager(LoginManager::getInstance(db)) // Might throw
{}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(this);
}

LoginManager* RequestHandlerFactory::getLoginManager() noexcept
{
    return this->m_loginManager;
}

// Singleton
RequestHandlerFactory* RequestHandlerFactory::getInstance(IDatabase* db)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_HandlerFactory == nullptr)
    {
        m_HandlerFactory = new RequestHandlerFactory(db);
    }
    return m_HandlerFactory;
}
