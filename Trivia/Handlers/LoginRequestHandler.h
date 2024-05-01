#pragma once

#include "IRequestHandler.h"

class RequestHandlerFactory; // Double-circular-jerk-dependency-linkage mega-shit

class LoginRequestHandler : public IRequestHandler
{
public:
    LoginRequestHandler(RequestHandlerFactory& handlerFactory);
    bool isRequestRelevant(const RequestInfo& info) override;
    RequestResult handleRequest(const RequestInfo& info) override;

private:
    RequestHandlerFactory& m_handlerFactory;
};
