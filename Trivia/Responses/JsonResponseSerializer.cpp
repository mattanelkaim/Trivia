#include "Helper.h"
#include "JsonResponseSerializer.h"
#include "ServerDefinitions.h"
#include <cstddef> // size_t
#include <string>
#include <string_view>
#include <iterator>
#include <map>

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
    json j;
    auto& jRooms = j[JsonFields::ROOMS] = json::object(); // Create the "highScores" wrapper

    for (const RoomData& room : response.rooms)
    {
        json value;
        value.emplace("name", room.name);
        value.emplace("maxPlayers", room.maxPlayers);
        value.emplace("questionCount", room.numOfQuestionsInGame);
        value.emplace("questionTimeout", room.timePerQuestion);
        value.emplace("status", room.status);

        jRooms.emplace(std::to_string(room.id), std::move(value));
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
    json j;
    auto& jHighScores = j[JsonFields::HIGH_SCORES] = json::object(); // Create the "highScores" wrapper

    uint16_t i = 0;
    for (auto it = response.statistics.cbegin(); i < NUM_TOP_SCORES; ++i)
    {
        json value;
        if (it != response.statistics.cend())
        {
            value.emplace("name", it->first);
            value.emplace("score", it->second);
        }
        else
        {
            value.emplace("name", "None");
            value.emplace("score", 0.0);
        }
        jHighScores.emplace(std::to_string(i + 1), std::move(value)); // Add the key-value pair

        if (it != response.statistics.cend())
            it++;
    }

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
                {SCORE,           std::stod(response.statistics[0], nullptr)},
                {TOTAL_GAMES,     std::stoi(response.statistics[1], nullptr)},
                {TOTAL_ANSWERS,   std::stoi(response.statistics[2], nullptr)},
                {CORRECT_ANSWERS, std::stoi(response.statistics[3], nullptr)}
            }
        }
    };

    return serializeGeneralResponse(ResponseCode::OK, j.dump());
}


// NOLINTNEXTLINE(bugprone-exception-escape) - ignore std::bad_alloc
buffer JsonResponseSerializer::serializeGeneralResponse(const ResponseCode type, const std::string_view json) noexcept
{
    // Directly constructing the buffer for efficiency
    return {
        std::from_range,
        // The first byte is the response code
        std::to_string(type) +
        // Pushing the JSON's length to the buffer
        Helper::getPaddedNumber(json.length(), BYTES_RESERVED_FOR_MSG_LEN) +
        // Pushing the actual message to the buffer
        json.data()
    };
}
