#pragma once

#include "../ServerDefenitions.h"

class JsonResponsePacketDeserializer
{
public:
	JsonResponsePacketDeserializer() = delete; // This ensures that this class is never instantiated

	static LoginRequest deserializeLoginResponse(const buffer& response);
	static SignupRequest deserializeSignupResponse(const buffer& response);
};
