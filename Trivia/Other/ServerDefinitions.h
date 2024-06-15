// NOLINTBEGIN(clang-diagnostic-unused-const-variable, clang-diagnostic-unused-macros)
#pragma once
#pragma warning(disable: 4820) // Padding added after data member

#include "LoggedUser.h"
#include <cstdint>
#include <ctime> // Used for time_t
#include <map>
#include <span>
#include <string>
#include <string_view>
#include <vector>

class IRequestHandler; // Double-circular-jerk-dependency-linkage mega-shit

using byte = unsigned char;
using buffer = std::vector<byte>;
using readonly_buffer = std::span<const byte>;


#pragma region IO

#define PRINT_IO true // Used in Helper
#define OUTPUT_COLORS true
#define SERVER_DEBUG true
#define EXTENDED_ERRORS true

#if OUTPUT_COLORS
    constexpr std::string_view ANSI_RED    = "\033[31;1m"; // Red and bold
    constexpr std::string_view ANSI_GREEN  = "\033[32;1m"; // Green and bold
    constexpr std::string_view ANSI_BLUE   = "\033[34;1m"; // Blue and bold
    constexpr std::string_view ANSI_NORMAL = "\033[0m"; // Resets back to default
#else
    constexpr std::string_view ANSI_RED;
    constexpr std::string_view ANSI_GREEN;
    constexpr std::string_view ANSI_BLUE;
    constexpr std::string_view ANSI_NORMAL;
#endif

#pragma endregion


#pragma region protocolDefinitions

/* protocol template: {code}{data length}{message}
*                      ^^^^  ^^^^^^^^^^^  ^^^^^^^
*                     1 byte,  4 bytes,  {data length} bytes
*  example: "10005hello"
*/

constexpr uint16_t PORT = 7777;

constexpr auto BYTES_RESERVED_FOR_CODE = 1;
constexpr auto BYTES_RESERVED_FOR_MSG_LEN = 4;

// A single-value enum to catch as an exception
enum ABORT_FLAG
{
    ABORT
};

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

#pragma endregion


#pragma region roomDefinitions

enum RoomStatus
{
    OPEN,
    CLOSED,
};

struct RoomData
{
    std::string name;
    uint32_t id;
    uint16_t maxPlayers;
    uint16_t numOfQuestionsInGame;
    uint32_t timePerQuestion;
    RoomStatus status;
};

#pragma endregion


#pragma region responseDefinitions

enum ResponseCode
{
    OK,
    // Login
    LOGIN_FAILED,
    // Signup
    USERNAME_ALREADY_EXISTS,
    // Join Room
    ROOM_IS_FULL,
    ROOM_IS_NOT_OPEN, // Either closed or in-game
    // General Errors
    ERR, // ERROR won't compile
    ERR_NOT_FOUND, // General error
};

// Response structs
struct ErrorResponse
{
    std::string message;
};

struct StatusResponse
{
    ResponseCode status;
};

// Are simply a response only with a status field
using LoginResponse = StatusResponse;
using SignupResponse = StatusResponse;
using LogoutResponse = StatusResponse;
using JoinRoomResponse = StatusResponse;
using CreateRoomResponse = StatusResponse;
using CloseRoomResponse = StatusResponse;
using StartRoomResponse = StatusResponse;
using LeaveRoomResponse = StatusResponse;

struct GetRoomsResponse : StatusResponse
{
    std::vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse : StatusResponse
{
    std::vector<LoggedUser> players;
};

struct GetHighScoreResponse : StatusResponse
{
    std::map<LoggedUser, double> statistics;
};

struct GetPersonalStatsResponse : StatusResponse
{
    std::vector<std::string> statistics;
};

struct GetRoomStateResponse : StatusResponse
{
    RoomStatus state;
    bool hasGameBegun;
    std::vector<LoggedUser> players;
    uint16_t questionCount;
    uint32_t answerTimeout;
};

namespace JsonFields
{
    constexpr std::string_view MESSAGE = "message";
    constexpr std::string_view STATUS = "status";
    constexpr std::string_view ROOMS = "rooms";
    constexpr std::string_view PLAYERS_IN_ROOM = "playersInRoom";
    constexpr std::string_view HIGH_SCORES = "highScores";
    constexpr std::string_view STATISTICS = "userStatistics";
    constexpr std::string_view ROOM_STATE = "roomState";

    namespace UserStats
    {
        constexpr std::string_view USERNAME = "name";
        constexpr std::string_view SCORE = "score";
        constexpr std::string_view TOTAL_GAMES = "games";
        constexpr std::string_view TOTAL_ANSWERS = "totalAnswers";
        constexpr std::string_view CORRECT_ANSWERS = "correctAnswers";
    } // namespace UserStats

    namespace RoomProperties
    {
        constexpr std::string_view ROOM_NAME = "name";
        constexpr std::string_view MAX_PLAYERS = "maxPlayers";
        constexpr std::string_view QUESTION_COUNT = "questionCount";
        constexpr std::string_view QUESTION_TIMEOUT = "questionTimeout";
        constexpr std::string_view ROOM_STATUS = "state";
        constexpr std::string_view HAS_BEGUN = "hasGameBegun";
    } // namespace RoomProperties
} // namespace JsonFields

#pragma endregion


#pragma region requestDefinitions

enum RequestId
{
    LOGIN,
    SIGNUP,
    GET_PLAYERS_IN_ROOM,
    JOIN_ROOM,
    CREATE_ROOM,
    GET_ROOMS,
    GET_STATISTICS,
    GET_HIGHSCORE,
    LOGOUT,
    START_ROOM,
    LEAVE_ROOM,
    CLOSE_ROOM,
    GET_ROOM_STATE,
};

struct RequestInfo
{
    RequestId id{};
    time_t receivalTime{};
    buffer buffer;
};

struct RequestResult
{
    buffer response;
    IRequestHandler* newHandler{nullptr};
};

// Request structs

struct Request {}; // Empty struct to inherit from

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
    uint16_t maxUsers;
    uint16_t questionCount;
    uint32_t answerTimeout;
};

struct CloseRoomRequest : Request
{
    uint32_t roomId;
};

struct StartGameRequest : Request
{
    uint32_t roomId;
};

struct GetRoomStateRequest : Request
{
    uint32_t roomId;
};

struct LeaveRoomRequest : Request
{
    uint32_t roomId;
};

#pragma endregion
// NOLINTEND(clang-diagnostic-unused-const-variable, clang-diagnostic-unused-macros)
