#pragma once

#include <ctime> // Used for std::time_t
#include <string>
#include <vector>

using byte = unsigned char;
using buffer = std::vector<byte>; // The 'byte' typedef already exists in <vector>

#pragma region DatabaseDefenitions

/* Hey @mattany funny bunny money honey runny, Please keep this as
* a #define and not using/constexpr/whatever other pakistani shit
* you like doing because it enables us to concatenate it with other
* literal strings if needed. For example: '"../" DB_FILE_NAME' 
* sincerely, daddy. */
constexpr std::string_view DB_FILE_NAME = "TriviaDB.sqlite";

#pragma endregion

#pragma region responseDefenitions

// Response structs
struct ErrorResponse
{
	std::string message;
};

struct LoginResponse
{
	unsigned int status;
};

struct SignupResponse
{
	unsigned int status;
};

constexpr std::string_view ERROR_MSG_JSON = "{message: \"ERROR\"}";

#pragma endregion

#pragma region protocolDefenitions

enum messageType : byte
{
	RESPONSE,
	REQUEST
};

constexpr auto BYTES_RESERVED_FOR_MSG_LEN = 4;

#pragma endregion
