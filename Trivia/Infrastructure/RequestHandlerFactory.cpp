#include "IDatabase.h"
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "LoggedUser.h"
#include "Room.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include <memory>
#include <mutex>


RequestHandlerFactory::RequestHandlerFactory(IDatabase* db) :
    m_database(db),
    m_loginManager(LoginManager::getInstance(db)), // Might throw
    m_roomManager(RoomManager::getInstance()), //...
    m_statisticsManager(StatisticsManager::getInstance(db)) //...
{}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& user)
{
    return new MenuRequestHandler(this->m_database, user);
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(this);
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser& user, Room room)
{
    return new RoomAdminRequestHandler(this->m_database, user, std::move(room));
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser& user, Room room)
{
    return new RoomMemberRequestHandler(this->m_database, user, std::move(room));
}

LoginManager* RequestHandlerFactory::getLoginManager() noexcept
{
    return this->m_loginManager;
}

StatisticsManager* RequestHandlerFactory::getStatisticsManager() noexcept
{
    return this->m_statisticsManager;
}

RoomManager* RequestHandlerFactory::getRoomManager() noexcept
{
    return this->m_roomManager;
}

// Singleton
RequestHandlerFactory* RequestHandlerFactory::getInstance(IDatabase* db)
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    if (m_HandlerFactory == nullptr) [[unlikely]]
    {
        m_HandlerFactory = std::unique_ptr<RequestHandlerFactory>(new RequestHandlerFactory(db));
    }
    return m_HandlerFactory.get();
}

