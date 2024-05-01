#pragma once

#include "IRequestHandler.h"

class RequestHandlerFactory; // Double-circular-jerk-dependency-linkage mega-shit

class LoginRequestHandler : public IRequestHandler
{
public:
    explicit LoginRequestHandler(RequestHandlerFactory& handlerFactory);
    bool isRequestRelevant(const RequestInfo& info) override;
    RequestResult handleRequest(const RequestInfo& info) override;

    // Helper functions
    RequestResult login(const RequestInfo& info);
    RequestResult signup(const RequestInfo& info);

private:
    RequestHandlerFactory& m_handlerFactory;
};
