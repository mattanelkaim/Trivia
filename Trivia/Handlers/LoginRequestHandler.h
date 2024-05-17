#pragma once

#include "IRequestHandler.h"

class RequestHandlerFactory; // Double-circular-jerk-dependency-linkage mega-shit

class LoginRequestHandler final : public IRequestHandler
{
public:
    explicit LoginRequestHandler(RequestHandlerFactory& handlerFactory) noexcept;
    bool isRequestRelevant(const RequestInfo& info) const noexcept override;
    RequestResult handleRequest(const RequestInfo& info) override;
    
    // Helper functions
    RequestResult login(const RequestInfo& info);
    RequestResult signup(const RequestInfo& info);

    // Avoid shitty compiler warnings
    LoginRequestHandler operator=(const LoginRequestHandler& other) = delete;

private:
    RequestHandlerFactory& m_handlerFactory;
};
