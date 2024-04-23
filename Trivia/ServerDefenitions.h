#pragma once

#include <vector>

#ifndef BUFFER
typedef std::vector<byte> buffer; // the 'byte' typedef already exists in <vector>
#define BUFFER
#endif // !BUFFER


#pragma region responseDefenitions

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

#define ERROR_MESSAGE_JSON "{message: \"ERROR\"}"
#define ERROR_MESSAGE_JSON_LENGTH sizeof(ERROR_MESSAGE_JSON)

#pragma endregion

#pragma region protocolDefenitions

enum messageType : byte
{
	RESPONSE,
	REQUEST
};

#define NUM_BYTES_RESERVED_FOR_MESSAGE_LENGTH 4

#pragma endregion