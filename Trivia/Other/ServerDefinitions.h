// NOLINTBEGIN(clang-diagnostic-unused-const-variable, clang-diagnostic-unused-macros)
#pragma once

#define SERVER_DEBUG_ALL // Debugging flag in Helper

#include <cstdint>
#include <ctime> // Used for time_t
#include <span>
#include <string>
#include <string_view>
#include <vector>

class IRequestHandler; // Double-circular-jerk-dependency-linkage mega-shit

using byte = unsigned char;
using buffer = std::vector<byte>;
using readonly_buffer = std::span<const byte>;


#pragma region IO

constexpr std::string_view ANSI_RED = "\033[31;1m"; // Red and bold
constexpr std::string_view ANSI_NORMAL = "\033[0m"; // Resets back to default

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


#pragma region DatabaseDefinitions

/* Hey @mattany funny bunny money honey runny, Please keep this as
* a #define and not using/constexpr/whatever other pakistani shit
* you like doing because it enables us to concatenate it with other
* literal strings if needed. For example: '"../" DB_FILE_NAME'
* sincerely, daddy. */
constexpr std::string_view DB_FILE_NAME = "TriviaDB.sqlite";
constexpr uint16_t NUM_POSSIBLE_ANSWERS_PER_QUESTION = 4;
constexpr uint16_t NUM_TOP_SCORES = 5;

constexpr auto DECIMAL_BASE = 10; // Dah, but needed for some str-to-integral converting shit

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


#pragma region responseDefinitions

// Response structs
struct ErrorResponse
{
    std::string message;
};

struct LoginResponse
{
    uint32_t status;
};

struct SignupResponse
{
    uint32_t status;
};

struct LogoutResponse
{
    uint32_t status;
};

struct GetRoomsResponse
{
    uint32_t status;
    std::vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse
{
    std::vector<std::string> players;
};

struct GetHighScoreResponse
{
    uint32_t status;
    std::vector<std::string> statistics;
};

struct GetPersonalStatsResponse
{
    uint32_t status;
    std::vector<std::string> statistics;
};

struct JoinRoomResponse
{
    uint32_t status;
};

struct CreateRoomResponse
{
    uint32_t status;
};

namespace JsonFields
{
    constexpr std::string_view MESSAGE = "message";
    constexpr std::string_view STATUS = "status";
    constexpr std::string_view ROOMS = "rooms";
    constexpr std::string_view PLAYERS_IN_ROOM = "playersInRoom";
    constexpr std::string_view HIGH_SCORES = "highScores";
    constexpr std::string_view STATISTICS = "userStatistics";

    namespace UserStats
    {
        constexpr std::string_view SCORE = "score";
        constexpr std::string_view TOTAL_GAMES = "games";
        constexpr std::string_view TOTAL_ANSWERS = "totalAnswers";
        constexpr std::string_view CORRECT_ANSWERS = "correctAnswers";
    } //namesapce UserStats
} //namesapce JsonFields

#pragma endregion


#pragma region requestDefinitions

enum RequestId : size_t
{
    LOGIN,
    SIGNUP,
    GET_PLAYERS_IN_ROOM,
    JOIN_ROOM,
    CREATE_ROOM
};

struct RequestInfo
{
    RequestId id{};
    time_t receivalTime{};
    buffer buffer;
};

struct Request {}; // Empty struct to inherit from

struct RequestResult : Request
{
    buffer response;
    IRequestHandler* newHandler{nullptr};
};

// Request structs
struct LoginRequest : Request
{
    std::string username;
    std::string password;
};

struct SignupRequest : Request
{
    std::string username;
    std::string password;
    std::string email;
};

struct GetPlayersInRoomRequest : Request
{
    uint32_t roomId;
};

struct JoinRoomRequest : Request
{
    uint32_t roomId;
};

struct CreateRoomRequest : Request
{
    std::string roomName;
    uint32_t maxUsers;
    uint32_t questionCount;
    uint32_t answerTimeout;
};

#pragma endregion
// NOLINTEND(clang-diagnostic-unused-const-variable, clang-diagnostic-unused-macros)
