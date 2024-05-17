#pragma once

#include "ServerDefinitions.h"

class JsonResponseSerializer
{
public:
    JsonResponseSerializer() = delete; // This ensures that this class is never instantiated

    static buffer serializeResponse(const ErrorResponse& response);
    static buffer serializeResponse(const LoginResponse response);
    static buffer serializeResponse(const SignupResponse response);

private:
    static buffer serializeGeneralResponse(const messageType type, const std::string_view json); // Internal helper function
};
