#pragma once

#include "ServerDefinitions.h"

class JsonResponseSerializer
{
public:
    JsonResponseSerializer() = delete; // This ensures that this class is never instantiated

    static buffer serializeErrorResponse(const ErrorResponse& response);
    static buffer serializeLoginResponse(const LoginResponse& response);
    static buffer serializeSignupResponse(const SignupResponse& response);

private:
    static buffer serializeGeneralResponse(const messageType& type, const std::string_view& json); // Internal helper function
};
