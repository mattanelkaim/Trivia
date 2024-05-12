#include "JsonRequestDeserializer.h"
#include "JsonResponseSerializer.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "ServerDefinitions.h"
#include <stdexcept>


LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory* handlerFactory) noexcept :
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
    LoginManager* loginManager = this->m_handlerFactory->getLoginManager();
    RequestResult result;

    const LoginRequest request = JsonResponseDeserializer::deserializeLoginResponse(info.buffer);
    if (loginManager->login(request.username, request.password)) [[likely]]
    {
        result.response = JsonResponseSerializer::serializeLoginResponse(LoginResponse{RESPONSE});
        result.newHandler = new MenuRequestHandler();
    }
    else [[unlikely]]
    {
        result.response = JsonResponseSerializer::serializeErrorResponse(ErrorResponse{"Login failed"});
        result.newHandler = new LoginRequestHandler(this->m_handlerFactory); // Retry login
    }

    return result;
}

RequestResult LoginRequestHandler::signup(const RequestInfo& info)
{
    LoginManager* loginManager = this->m_handlerFactory->getLoginManager();
    RequestResult result;

    const SignupRequest request = JsonResponseDeserializer::deserializeSignupResponse(info.buffer);
    if (loginManager->signup(request.username, request.password, request.email)) [[likely]]
    {
        result.response = JsonResponseSerializer::serializeSignupResponse(SignupResponse{RESPONSE});
        result.newHandler = new MenuRequestHandler();
    }
    else [[unlikely]]
    {
        result.response = JsonResponseSerializer::serializeErrorResponse(ErrorResponse{"Signup failed"});
        result.newHandler = new LoginRequestHandler(this->m_handlerFactory); // Retry signup
    }

    return result;
}
