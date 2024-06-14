#pragma once

#include "IRequestHandler.h"
#include "ServerDefinitions.h"

class RequestHandlerFactory; // Double-circular-jerk-dependency-linkage mega-shit

class LoginRequestHandler final : public IRequestHandler
{
public:
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
    /*######################################
    ############ HELPER METHODS ############
    ######################################*/

    /**
    * @throws InvalidProtocolStructure
    * @throws InvalidSQL
    */
    static RequestResult login(const RequestInfo& info);

    /**
    * @throws InvalidProtocolStructure
    * @throws InvalidSQL
    */
    static RequestResult signup(const RequestInfo& info);
};
