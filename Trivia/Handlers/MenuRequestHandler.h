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
    RequestResult logout([[maybe_unused]] const RequestInfo& info);
    RequestResult getRooms ([[maybe_unused]] const RequestInfo& info);
    RequestResult getPlayersInRoom([[maybe_unused]] const RequestInfo& info);
    RequestResult getPersonalStats([[maybe_unused]] const RequestInfo& info);
    RequestResult getHighScore([[maybe_unused]] const RequestInfo& info);
    RequestResult joinRoom([[maybe_unused]] const RequestInfo& info);
    RequestResult createRoom([[maybe_unused]] const RequestInfo& info);

        // Members
    LoggedUser m_user;
    std::shared_ptr<RequestHandlerFactory> m_handlerFactory;



};
