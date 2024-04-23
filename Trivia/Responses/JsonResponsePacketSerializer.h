#pragma once

#include <string>
#include "../ServerDefenitions.h"

class JsonResponsePacketSerializer
{
public:
	JsonResponsePacketSerializer() = delete; // this ensures that this class is never instanciated

	static buffer serializeErrorResponse(const ErrorResponse& response);
	static buffer serializeLoginResponse(const LoginResponse& response);
	static buffer serializeSignupResponse(const SignupResponse& response);
};