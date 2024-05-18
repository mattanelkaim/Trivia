#include "JsonRequestDeserializer.hpp"
#include "JsonResponseSerializer.h"
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "ServerDefinitions.h"
#include <stdexcept>


LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& handlerFactory) noexcept :
    m_handlerFactory(handlerFactory)
{}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& info) const noexcept
{
    return info.id == LOGIN || info.id == SIGNUP;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& info)
{
    switch (info.id)
    {
    [[likely]] case LOGIN:
        return this->login(info);
    case SIGNUP:
        return this->signup(info);
    default:
        throw std::runtime_error("RequestInfo is not login/signup!");
    }
}


// HELPER FUNCTIONS


RequestResult LoginRequestHandler::login(const RequestInfo& info)
{
    const auto request = JsonRequestDeserializer::deserializeRequest<LoginRequest>(info.buffer);

    LoginManager* loginManager = this->m_handlerFactory.getLoginManager();
    if (loginManager->login(request.username, request.password)) [[likely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(LoginResponse{RESPONSE}),
            .newHandler = new MenuRequestHandler()
        };
    }
    else [[unlikely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(ErrorResponse{"Login failed"}),
            .newHandler = new LoginRequestHandler(this->m_handlerFactory) // Retry login
        };
    }
}

RequestResult LoginRequestHandler::signup(const RequestInfo& info)
{
    const auto request = JsonRequestDeserializer::deserializeRequest<SignupRequest>(info.buffer);

    LoginManager* loginManager = this->m_handlerFactory.getLoginManager();
    if (loginManager->signup(request.username, request.password, request.email)) [[likely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(SignupResponse{RESPONSE}),
            .newHandler = new MenuRequestHandler()
        };
    }
    else [[unlikely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(ErrorResponse{"Signup failed"}),
            .newHandler = new LoginRequestHandler(this->m_handlerFactory) // Retry signup
        };
    }
}
