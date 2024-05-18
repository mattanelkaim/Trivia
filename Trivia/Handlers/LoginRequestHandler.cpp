#include "JsonRequestDeserializer.hpp"
#include "JsonResponseSerializer.h"
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
    RequestResult result;
    const LoginRequest request = JsonRequestDeserializer::deserializeRequest<LoginRequest>(info.buffer);

    const std::unique_ptr<LoginManager>& loginManager = this->m_handlerFactory.getLoginManager();
    if (loginManager->login(request.username, request.password)) [[likely]]
    {
        result.response = JsonResponseSerializer::serializeResponse(LoginResponse{RESPONSE});
        result.newHandler = new MenuRequestHandler();
    }
    else [[unlikely]]
    {
        result.response = JsonResponseSerializer::serializeResponse(ErrorResponse{"Login failed"});
        result.newHandler = new LoginRequestHandler(this->m_handlerFactory); // Retry login
    }

    return result;
}

RequestResult LoginRequestHandler::signup(const RequestInfo& info)
{
    RequestResult result;
    const SignupRequest request = JsonRequestDeserializer::deserializeRequest<SignupRequest>(info.buffer);

    const std::unique_ptr<LoginManager>& loginManager = this->m_handlerFactory.getLoginManager();
    if (loginManager->signup(request.username, request.password, request.email)) [[likely]]
    {
        result.response = JsonResponseSerializer::serializeResponse(SignupResponse{RESPONSE});
        result.newHandler = new MenuRequestHandler();
    }
    else [[unlikely]]
    {
        result.response = JsonResponseSerializer::serializeResponse(ErrorResponse{"Signup failed"});
        result.newHandler = new LoginRequestHandler(this->m_handlerFactory); // Retry signup
    }

    return result;
}
