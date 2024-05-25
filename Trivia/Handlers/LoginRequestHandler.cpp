#include "JsonRequestDeserializer.hpp"
#include "JsonResponseSerializer.h"
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "ServerDefinitions.h"
#include "ServerException.h"
#if SERVER_DEBUG
#include <iostream>
#endif


LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory* handlerFactory) noexcept :
    m_handlerFactory(handlerFactory)
{}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& info) const noexcept
{
    return info.id == LOGIN || info.id == SIGNUP;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& info) noexcept
{
    try
    {
        switch (info.id)
        {
        [[likely]] case LOGIN:
            return this->login(info);
        case SIGNUP:
            return this->signup(info);
        default:
            throw InvalidProtocolStructure("RequestInfo is not login/signup!");
        }
    }
    catch (const ServerException& e) // Either InvalidProtocolStructure or InvalidSQL
    {
        if constexpr (SERVER_DEBUG)
            std::cerr << ANSI_RED << e.what() << ANSI_NORMAL << '\n';
        
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(ErrorResponse{"Invalid protocol structure"}),
            .newHandler = nullptr
        };
    }
}


// HELPER FUNCTIONS


RequestResult LoginRequestHandler::login(const RequestInfo& info)
{
    const auto request = JsonRequestDeserializer::deserializeRequest<LoginRequest>(info.buffer);

    LoginManager* loginManager = this->m_handlerFactory->getLoginManager();
    if (loginManager->login(request.username, request.password)) [[likely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(LoginResponse{OK}),
            .newHandler = new MenuRequestHandler()
        };
    }
    else [[unlikely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(ErrorResponse{"Login failed"}),
            .newHandler = nullptr // Retry login
        };
    }
}

RequestResult LoginRequestHandler::signup(const RequestInfo& info)
{
    const auto request = JsonRequestDeserializer::deserializeRequest<SignupRequest>(info.buffer);

    LoginManager* loginManager = this->m_handlerFactory->getLoginManager();
    if (loginManager->signup(request.username, request.password, request.email)) [[likely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(SignupResponse{OK}),
            .newHandler = new MenuRequestHandler()
        };
    }
    else [[unlikely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(ErrorResponse{"Signup failed"}),
            .newHandler = nullptr // Retry signup
        };
    }
}
