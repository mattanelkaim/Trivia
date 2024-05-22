#pragma once

#include "IRequestHandler.h"
#include "ServerDefinitions.h"
#include "LoggedUser.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory; // Double-circular-jerk-dependency-linkage mega-shit


class MenuRequestHandler final : public IRequestHandler
{
public:
    MenuRequestHandler() = default;
    bool isRequestRelevant(const RequestInfo& info) const noexcept override;
    RequestResult handleRequest(const RequestInfo& info) override;

private:    
    RequestResult logout(const RequestInfo& info);
    RequestResult getRooms (const RequestInfo& info);
    RequestResult getPlayersInRoom(const RequestInfo& info);
    RequestResult getPersonalStats(const RequestInfo& info);
    RequestResult getHighScore(const RequestInfo& info);
    RequestResult joinRoom(const RequestInfo& info);
    RequestResult createRoom(const RequestInfo& info);

        // Members
    LoggedUser m_user;
    std::shared_ptr<RequestHandlerFactory> m_handlerFactory;



};
