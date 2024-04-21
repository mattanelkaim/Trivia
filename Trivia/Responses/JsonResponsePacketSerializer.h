#pragma once

#include <string>
#include "../ServerDefenitions.h"

// response structs
struct ErrorResponse {
	std::string message;
};
struct LoginResponse {
	unsigned int status;
};
struct SignupResponse {
	unsigned int status;
};

class JsonResponsePacketSerializer
{
public:
	JsonResponsePacketSerializer() = delete; // this ensures that this class is never instanciated

	static buffer serializeResponse(const ErrorResponse& response);
	static buffer serializeResponse(const LoginResponse& response);
	static buffer serializeResponse(const SignupResponse& response);
};