#pragma once

#include "IRequestHandler.h"
#include "ServerDefinitions.h"

class RequestHandlerFactory; // Double-circular-jerk-dependency-linkage mega-shit

class LoginRequestHandler final : public IRequestHandler
{
public:
    explicit LoginRequestHandler(RequestHandlerFactory* handlerFactory) noexcept;

    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    bool isRequestRelevant(const RequestInfo& info) const noexcept override;
    RequestResult handleRequest(const RequestInfo& info) noexcept override;
    
    /*######################################
    #### AVOID SHITTY COMPILER WARNINGS ####
    ######################################*/

    LoginRequestHandler operator=(const LoginRequestHandler& other) = delete;

private:
    RequestHandlerFactory* m_handlerFactory;

    /*######################################
    ############ HELPER METHODS ############
    ######################################*/

    /**
    * @throws InvalidProtocolStructure
    * @throws InvalidSQL
    */
    RequestResult login(const RequestInfo& info);

    /**
    * @throws InvalidProtocolStructure
    * @throws InvalidSQL
    */
    RequestResult signup(const RequestInfo& info);
};
