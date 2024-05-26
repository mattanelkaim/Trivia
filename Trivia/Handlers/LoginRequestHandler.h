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

    explicit LoginRequestHandler(RequestHandlerFactory* handlerFactory) noexcept;
    bool isRequestRelevant(const RequestInfo& info) const noexcept override;
    RequestResult handleRequest(const RequestInfo& info) noexcept override;
    
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

    /*######################################
    #### AVOID SHITTY COMPILER WARNINGS ####
    ######################################*/

    LoginRequestHandler operator=(const LoginRequestHandler& other) = delete;

private:
    RequestHandlerFactory* m_handlerFactory;
};
