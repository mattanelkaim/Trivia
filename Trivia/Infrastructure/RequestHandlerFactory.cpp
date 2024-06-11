#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "RoomManager.h"
#include "StatisticsManager.h"


RequestHandlerFactory::RequestHandlerFactory() :
    m_loginManager(LoginManager::getInstance()), // Might throw
    m_roomManager(RoomManager::getInstance()), //...
    m_statisticsManager(StatisticsManager::getInstance()) //...
{}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& user)
{
    return new MenuRequestHandler(user);
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(this);
}

LoginManager* RequestHandlerFactory::getLoginManager() noexcept
{
    return this->m_loginManager;
}

// Singleton
RequestHandlerFactory* RequestHandlerFactory::getInstance()
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    if (m_HandlerFactory == nullptr) [[unlikely]]
    {
        m_HandlerFactory = std::unique_ptr<RequestHandlerFactory>(new RequestHandlerFactory());
    }
    return m_HandlerFactory.get();
}
