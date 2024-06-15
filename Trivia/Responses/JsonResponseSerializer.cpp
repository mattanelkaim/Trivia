#include "Helper.h"
#include "JsonResponseSerializer.h"
#include "ServerDefinitions.h"
#include <cstddef> // size_t
#include <cstdint>
#include <ranges>
#include <string>
#include <string_view>
#include <utility> // std::move

using std::to_string;

// NOLINTNEXTLINE(bugprone-exception-escape) - ignore json constructor
buffer JsonResponseSerializer::serializeResponse(const ErrorResponse& response) noexcept
{
    try
    {
        const json j{{JsonFields::MESSAGE, response.message}};
        return serializeGeneralResponse(ResponseCode::ERR, j.dump());
    }
    catch (const nlohmann::json_abi_v3_11_3::detail::type_error& e)
    {
        return serializeGeneralResponse(ResponseCode::ERR, e.what());
    }
}

// NOLINTNEXTLINE(bugprone-exception-escape) - ignore json constructor
buffer JsonResponseSerializer::serializeResponse(const GetRoomsResponse& response) noexcept
{
    using namespace JsonFields;
    using namespace RoomProperties;

    json j; // Wrapper
    auto& jRooms = j[JsonFields::ROOMS] = json::object(); // Create the "highScores" wrapper

    for (const RoomData& room : response.rooms)
    {
        json data // Not const because we need to move it
        {
            {ROOM_NAME, room.name},
            {MAX_PLAYERS, room.maxPlayers},
            {QUESTION_COUNT, room.numOfQuestionsInGame},
            {QUESTION_TIMEOUT, room.timePerQuestion},
            {ROOM_STATUS, room.status}
        };

        jRooms.emplace(to_string(room.id), std::move(data));
    }

    return serializeGeneralResponse(ResponseCode::OK, j.dump());
}

// NOLINTNEXTLINE(bugprone-exception-escape) - ignore json constructor
buffer JsonResponseSerializer::serializeResponse(const GetPlayersInRoomResponse& response) noexcept
{
    const json j{{JsonFields::PLAYERS_IN_ROOM, response.players}};
    return serializeGeneralResponse(ResponseCode::OK, j.dump());
}

// NOLINTNEXTLINE(bugprone-exception-escape) - ignore json constructor
buffer JsonResponseSerializer::serializeResponse(const GetHighScoreResponse& response) noexcept
{
    using namespace JsonFields;
    using namespace JsonFields::UserStats;

    json j;
    auto& jHighScores = j[JsonFields::HIGH_SCORES] = json::object(); // Create the "highScores" wrapper

    uint16_t i = 0;

    // Add the highscore to the JSON object
    for (const auto& [name, score] : response.statistics)
        jHighScores.emplace(to_string(++i), json{{USERNAME, name}, {SCORE, score}});

    // Fill in the rest with default data
    while (i < NUM_TOP_SCORES)
        jHighScores.emplace(to_string(++i), json{{USERNAME, "None"}, {SCORE, 0.0}});

    return serializeGeneralResponse(ResponseCode::OK, j.dump());
}

// NOLINTNEXTLINE(bugprone-exception-escape) - ignore json constructor
buffer JsonResponseSerializer::serializeResponse(const GetPersonalStatsResponse& response) noexcept
{
    using namespace JsonFields;
    using namespace JsonFields::UserStats;

    // Sub-fields that construct the "userStatistics" outer field
    const json j
    {
        {
            STATISTICS,
            {
                {SCORE,           std::stod(response.statistics[0])},
                {TOTAL_GAMES,     std::stoi(response.statistics[1])},
                {TOTAL_ANSWERS,   std::stoi(response.statistics[2])},
                {CORRECT_ANSWERS, std::stoi(response.statistics[3])}
            }
        }
    };

    return serializeGeneralResponse(ResponseCode::OK, j.dump());
}

// NOLINTNEXTLINE(bugprone-exception-escape) - ignore json constructor
buffer JsonResponseSerializer::serializeResponse(const GetRoomStateResponse& response) noexcept
{
    using namespace JsonFields;
    using namespace RoomProperties;

    // Sub-fields that construct the "userStatistics" outer field
    const json j
    {
        {
            ROOM_STATE,
            {                                
                {QUESTION_COUNT, response.questionCount},
                {QUESTION_TIMEOUT, response.answerTimeout},
                {ROOM_STATUS, response.state},
                {PLAYERS_IN_ROOM, response.players},
                {HAS_BEGUN, response.hasGameBegun}
            }
        }
    };

    return serializeGeneralResponse(ResponseCode::OK, j.dump());
}

// NOLINTNEXTLINE(bugprone-exception-escape) - ignore std::bad_alloc
constexpr buffer JsonResponseSerializer::serializeGeneralResponse(const ResponseCode type, const std::string_view json) noexcept
{
    // Directly constructing the buffer for efficiency
    return {
        std::from_range,
        // The first byte is the response code
        to_string(type) + \
        // Pushing the JSON's length to the buffer
        Helper::getPaddedNumber(json.length(), BYTES_RESERVED_FOR_MSG_LEN) + \
        // Pushing the actual message to the buffer
        json.data()
    };
}
