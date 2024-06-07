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
    // Join all string fields with a delimiter
    std::string rooms;
    if (!response.rooms.empty()) [[unlikely]] // Avoid resizing to a negative size
    {
        for (const RoomData& room : response.rooms)
            rooms += room.name + ", ";
        rooms.resize(rooms.size() - 2); // Delete last 2 chars (", ")
    }

    const json j{{JsonFields::ROOMS, rooms}};
    return serializeGeneralResponse(ResponseCode::OK, j.dump());
}

// NOLINTNEXTLINE(bugprone-exception-escape) - ignore json constructor
buffer JsonResponseSerializer::serializeResponse(const GetPlayersInRoomResponse& response) noexcept
{
    // Join all strings with a delimiter
    std::string players;
    if (!response.players.empty()) [[unlikely]] // Avoid resizing to a negative size
    {
        for (const std::string& room : response.players)
            players += room + ", ";
        players.resize(players.size() - 2); // Delete last 2 chars (", ")
    }

    const json j{{JsonFields::PLAYERS_IN_ROOM, players}};
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
