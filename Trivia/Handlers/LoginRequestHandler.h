#pragma once

#include "IRequestHandler.h"
#include "ServerDefinitions.h"
#include <optional>

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

    static constexpr std::optional<RequestResult> validateSignupCredentials(const SignupRequest& request) noexcept;
};
