#pragma once

#include "IRequestHandler.h"
#include "ServerDefinitions.h"
#include "LoggedUser.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory; // Double-circular-jerk-dependency-linkage mega-shit


class MenuRequestHandler final : public IRequestHandler
{
public:
    explicit MenuRequestHandler(IDatabase* db, const LoggedUser& user);
    bool isRequestRelevant(const RequestInfo& info) const noexcept override;
    RequestResult handleRequest(const RequestInfo& info) override;

private:    
    RequestResult logout([[maybe_unused]] const RequestInfo& info) const noexcept;
    RequestResult getRooms([[maybe_unused]] const RequestInfo& info) const noexcept;
    RequestResult getPlayersInRoom([[maybe_unused]] const RequestInfo& info) const noexcept;
    RequestResult getPersonalStats([[maybe_unused]] const RequestInfo& info) const noexcept;
    RequestResult getHighScore([[maybe_unused]] const RequestInfo& info) const noexcept;
    RequestResult joinRoom(const RequestInfo& info) const noexcept;
    RequestResult createRoom(const RequestInfo& info) const noexcept;

    // Members    
    std::shared_ptr<RequestHandlerFactory> m_handlerFactory;
    std::shared_ptr<RoomManager> m_roomManager;
    std::shared_ptr<StatisticsManager> m_statisticsManager;
    const LoggedUser m_user;
};
