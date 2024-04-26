#pragma once

#include "../ServerDefenitions.h"

class JsonResponsePacketSerializer
{
public:
	JsonResponsePacketSerializer() = delete; // This ensures that this class is never instantiated

	static buffer serializeErrorResponse(const ErrorResponse& response);
	static buffer serializeLoginResponse(const LoginResponse& response);
	static buffer serializeSignupResponse(const SignupResponse& response);
};
