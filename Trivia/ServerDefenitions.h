#pragma once

#include <ctime> // Used for std::time_t
#include <string>
#include <vector>

class IRequestHandler; // Double-circular-jerk-dependency-linkage mega-shit

using byte = unsigned char;
using buffer = std::vector<byte>;


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


#pragma region requestDefenitions

enum RequestId : size_t
{
    LOGIN,
    SIGNUP
};

struct RequestInfo
{
    RequestId id{};
    std::time_t receivalTime{};
    buffer buffer;
};

struct RequestResult
{
    buffer response;
    IRequestHandler* newHandler = nullptr;
};

// Request structs
struct LoginRequest
{
    std::string username;
    std::string password;
};

struct SignupRequest
{
    std::string username;
    std::string password;
    std::string email;
};

#pragma endregion


#pragma region protocolDefenitions

/* protocol template: {code}{data length}{message}
 *					   ^^^^  ^^^^^^^^^^^  ^^^^^^^
 *                   1 byte,  4 bytes,  {data length} bytes
 */

enum messageType : byte
{
    RESPONSE,
    REQUEST
};

constexpr auto BYTES_RESERVED_FOR_CODE = 1;
constexpr auto BYTES_RESERVED_FOR_MSG_LEN = 4;
constexpr auto JSON_OFFSET = BYTES_RESERVED_FOR_MSG_LEN + 1; // + msg code

#pragma endregion
