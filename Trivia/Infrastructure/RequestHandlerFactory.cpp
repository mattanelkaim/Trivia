#include "IDatabase.h"
#include "LoginManager.h"
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
    return this->m_loginManager.get();
}

// Singleton
std::shared_ptr<RequestHandlerFactory> RequestHandlerFactory::getInstance(IDatabase* db)
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    if (m_HandlerFactory == nullptr) [[unlikely]]
    {
        m_HandlerFactory = std::shared_ptr<RequestHandlerFactory>(new RequestHandlerFactory(db));
    }
    return m_HandlerFactory;
}

