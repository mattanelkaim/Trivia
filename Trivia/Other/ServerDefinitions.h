// NOLINTBEGIN(clang-diagnostic-unused-const-variable, clang-diagnostic-unused-macros)

#pragma once
#define SERVER_DEBUG_ALL // Debugging flag in Helper

#include <ctime> // Used for std::time_t
#include <span>
#include <string>
#include <vector>

class IRequestHandler; // Double-circular-jerk-dependency-linkage mega-shit

using byte = unsigned char;
using buffer = std::vector<byte>;
using readonly_buffer = std::span<const byte>;


#pragma region IO

constexpr std::string_view ANSI_RED = "\033[31;1m"; // Red and bold
constexpr std::string_view ANSI_NORMAL = "\033[0m"; // Resets back to default

#pragma endregion


#pragma region DatabaseDefinitions

/* Hey @mattany funny bunny money honey runny, Please keep this as
* a #define and not using/constexpr/whatever other pakistani shit
* you like doing because it enables us to concatenate it with other
* literal strings if needed. For example: '"../" DB_FILE_NAME'
* sincerely, daddy. */
constexpr std::string_view DB_FILE_NAME = "TriviaDB.sqlite";

#pragma endregion


#pragma region responseDefinitions

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


#pragma region requestDefinitions

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


#pragma region protocolDefinitions

/* protocol template: {code}{data length}{message}
 *                     ^^^^  ^^^^^^^^^^^  ^^^^^^^
 *                   1 byte,  4 bytes,  {data length} bytes
 */

enum messageType : byte
{
    REQUEST,
    RESPONSE
};

constexpr uint16_t PORT = 7777;

constexpr auto BYTES_RESERVED_FOR_CODE = 1;
constexpr auto BYTES_RESERVED_FOR_MSG_LEN = 4;
constexpr auto JSON_OFFSET = BYTES_RESERVED_FOR_MSG_LEN + 1; // + msg code

constexpr int CLIENT_CLOSED_UNEXPECTEDLY = 10054; // WinError constant

#pragma endregion


#pragma region roomDefinitions

struct RoomData
{
    std::string name;
    uint32_t id;
    uint16_t maxPlayers;
    uint16_t numOfQuestionsInGame;
    uint32_t timePerQuestion;
    uint32_t status;
};

#pragma endregion

// NOLINTEND(clang-diagnostic-unused-const-variable, clang-diagnostic-unused-macros)
