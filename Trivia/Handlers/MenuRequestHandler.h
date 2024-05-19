#pragma once

#include "IRequestHandler.h"
#include "ServerDefinitions.h"
#include "LoggedUser.h"
#include "RequestHandlerFactory.h"

class MenuRequestHandler final : public IRequestHandler
{
public:
    bool isRequestRelevant(const RequestInfo& info) const noexcept override;
    RequestResult handleRequest(const RequestInfo& info) override;

private:    
    RequestResult handleRequest(RequestInfo);
    RequestResult signout(RequestInfo);
    RequestResult getRooms(RequestInfo);
    RequestResult getPlayersInRoom(RequestInfo);
    RequestResult getPersonalStats(RequestInfo);
    RequestResult getHighScore(RequestInfo);
    RequestResult joinRoom(RequestInfo);
    RequestResult createRoom(RequestInfo);

        // Members
    LoggedUser m_user;
    std::unique_ptr<RequestHandlerFactory>& m_handlerFactory;



};
