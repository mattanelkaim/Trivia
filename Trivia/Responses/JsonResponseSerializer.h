#pragma once

#include "ServerDefinitions.h"

class JsonResponseSerializer
{
public:
    JsonResponseSerializer() = delete; // This ensures that this class is never instantiated

    static buffer serializeErrorResponse(const ErrorResponse& response);
    static buffer serializeLoginResponse(LoginResponse response);
    static buffer serializeSignupResponse(SignupResponse response);

private:
    static buffer serializeGeneralResponse(messageType type, std::string_view json); // Internal helper function
};
