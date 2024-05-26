#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "LoginManager.h"
#include "IDatabase.h"
#include "RoomManager.h"
#include "StatisticsManager.h"


RequestHandlerFactory::RequestHandlerFactory(IDatabase* db) :
    m_database(db),
    m_loginManager(LoginManager::getInstance(db)), // Might throw
    m_roomManager(RoomManager::getInstance()), //...
    m_statisticsManager(StatisticsManager::getInstance(db)) //...
{}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& user)
{
    return new MenuRequestHandler(this->m_database);
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(*this);
}

LoginManager* RequestHandlerFactory::getLoginManager() noexcept
{
    return this->m_loginManager.get();
}

StatisticsManager* RequestHandlerFactory::getStatisticsManager() noexcept
{
    return this->m_statisticsManager.get();
}

RoomManager* RequestHandlerFactory::getRoomManager() noexcept
{
    return this->m_roomManager.get();
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
