#pragma warning(disable: 4061) // Enumerator in switch of enum is not explicitly handled by a case label

#include "Helper.h"
#include "InvalidProtocolStructure.h"
#include "JsonRequestDeserializer.hpp"
#include "JsonResponseSerializer.h"
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "ServerDefinitions.h"
#include "ServerException.h"
#include <optional>
#if SERVER_DEBUG
#include <iostream>
#endif


bool LoginRequestHandler::isRequestRelevant(const RequestInfo& info) const noexcept
{
    switch (info.id)
    {
        case LOGIN:
        case SIGNUP:
            return true;
        default:
            return false;
    }
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
            .newHandler = RequestHandlerFactory::createMenuRequestHandler(request.username)
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
    
    // Validate credentials
    const auto isValidResult = LoginRequestHandler::validateSignupCredentials(request);
    if (isValidResult.has_value()) [[unlikely]]
        return isValidResult.value();

    // Actually try to signup
    LoginManager& loginManager = LoginManager::getInstance();
    if (loginManager.signup(request.username, request.password, request.email)) [[likely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(SignupResponse{OK}),
            .newHandler = RequestHandlerFactory::createMenuRequestHandler(request.username)
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

constexpr std::optional<RequestResult> LoginRequestHandler::validateSignupCredentials(const SignupRequest& request) noexcept
{
    if (!Helper::isUsernameValid(request.username)) [[unlikely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(LoginResponse{INVALID_USERNAME}),
            .newHandler = nullptr // Retry login
        };
    }
    if (!Helper::isPasswordValid(request.password)) [[unlikely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(LoginResponse{INVALID_PASSWORD}),
            .newHandler = nullptr // Retry login
        };
    }
    if (!Helper::isEmailValid(request.email)) [[unlikely]]
    {
        return RequestResult{
            .response = JsonResponseSerializer::serializeResponse(LoginResponse{INVALID_EMAIL}),
            .newHandler = nullptr // Retry login
        };
    }

    return std::nullopt;
}
