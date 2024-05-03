#pragma once

#include "ServerDefenitions.h"

class JsonResponseDeserializer
{
public:
    JsonResponseDeserializer() = delete; // This ensures that this class is never instantiated

    static LoginRequest deserializeLoginResponse(const buffer& response);
    static SignupRequest deserializeSignupResponse(const buffer& response);
};
