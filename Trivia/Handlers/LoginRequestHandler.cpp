#pragma warning(disable: 4061) // Enumerator in switch of enum is not explicitly handled by a case label

#include "InvalidProtocolStructure.h"
#include "JsonRequestDeserializer.hpp"
#include "JsonResponseSerializer.h"
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "ServerDefinitions.h"
#include "ServerException.h"
#if SERVER_DEBUG
#include <iostream>
#endif


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
            return LoginRequestHandler::login(info);
        case SIGNUP:
            return LoginRequestHandler::signup(info);
        default:
            throw InvalidProtocolStructure("Request is not relevant to LoginRequestHandler!");
        }
    }
    catch (const ServerException& e) // Either InvalidProtocolStructure or InvalidSQL
    {
        if constexpr (SERVER_DEBUG)
            std::cerr << ANSI_RED << Helper::formatError(__FUNCTION__, e.what()) << ANSI_NORMAL << '\n';
        
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(ErrorResponse{"Invalid protocol structure"}),
            .newHandler = nullptr
        };
    }
}


/*######################################
############ HELPER METHODS ############
######################################*/


RequestResult LoginRequestHandler::login(const RequestInfo& info)
{
    const auto request = JsonRequestDeserializer::deserializeRequest<LoginRequest>(info.buffer);

    LoginManager& loginManager = LoginManager::getInstance();
    if (loginManager.login(request.username, request.password)) [[likely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(LoginResponse{OK}),
            .newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(request.username)
        };
    }
    else [[unlikely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(LoginResponse{LOGIN_FAILED}),
            .newHandler = nullptr // Retry login
        };
    }
}

RequestResult LoginRequestHandler::signup(const RequestInfo& info)
{
    const auto request = JsonRequestDeserializer::deserializeRequest<SignupRequest>(info.buffer);

    LoginManager& loginManager = LoginManager::getInstance();
    if (loginManager.signup(request.username, request.password, request.email)) [[likely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(SignupResponse{OK}),
            .newHandler = RequestHandlerFactory::getInstance().createMenuRequestHandler(request.username)
        };
    }
    else [[unlikely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(SignupResponse{USERNAME_ALREADY_EXISTS}),
            .newHandler = nullptr // Retry signup
        };
    }
}
