#pragma once

#include "ServerDefinitions.h"

class JsonResponseDeserializer
{
public:
    JsonResponseDeserializer() = delete; // This ensures that this class is never instantiated

    static LoginRequest deserializeLoginResponse(readonly_buffer response);
    static SignupRequest deserializeSignupResponse(readonly_buffer response);
};
