#pragma once
#define SERVER_DEBUG_ALL // Debugging flag in Helper

#include <ctime> // Used for std::time_t
#include <string>
#include <vector>

class IRequestHandler; // Double-circular-jerk-dependency-linkage mega-shit

using byte = unsigned char;
using buffer = std::vector<byte>;

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

namespace JsonFields
{
    constexpr std::string_view MESSAGE_FIELD = "message";
    constexpr std::string_view STATUS_FIELD = "status";
}


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
