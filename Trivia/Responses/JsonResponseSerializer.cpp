#include "Helper.h"
#include "JsonResponseSerializer.h"
#include "ServerDefinitions.h"
#include <cstddef> // size_t
#include <string>
#include <string_view>

buffer JsonResponseSerializer::serializeResponse(const ErrorResponse& response)
{
    const json j{{JsonFields::MESSAGE, response.message}};
    return serializeGeneralResponse(ResponseCode::ERR, j.dump());
}

buffer JsonResponseSerializer::serializeResponse(const GetRoomsResponse& response) 
{
    // Join all string fields with a delimiter
    std::string rooms;
    for (const RoomData& room : response.rooms)
        rooms += room.name + ", ";
    rooms.resize(rooms.size() - 2); // Delete last 2 chars (", ")

    const json j{{JsonFields::ROOMS, rooms}};
    return serializeGeneralResponse(ResponseCode::OK, j.dump());
}

buffer JsonResponseSerializer::serializeResponse(const GetPlayersInRoomResponse& response)
{
    // Join all strings with a delimiter
    std::string players;
    for (const std::string& room : response.players)
        players += room + ", ";
    players.resize(players.size() - 2); // Delete last 2 chars (", ")

    const json j{{JsonFields::PLAYERS_IN_ROOM, players}};
    return serializeGeneralResponse(ResponseCode::OK, j.dump());
}

buffer JsonResponseSerializer::serializeResponse(const GetHighScoreResponse& response)
{
    json j;
    
    // Fill json with inner fields, or with "None" if non-existent
    for (size_t i = 0; i < NUM_TOP_SCORES; ++i)
        j[JsonFields::HIGH_SCORES][i + 1] = (i < response.statistics.size()) ? response.statistics[i] : "None";

    return serializeGeneralResponse(ResponseCode::OK, j.dump());
}

buffer JsonResponseSerializer::serializeResponse(const GetPersonalStatsResponse& response)
{
    using namespace JsonFields;
    using namespace JsonFields::UserStats;

    // Sub-fields that construct the "userStatistics" outer field
    const json j
    {
        {
            STATISTICS,
            {
                {SCORE,           response.statistics[0]},
                {TOTAL_GAMES,     response.statistics[1]},
                {TOTAL_ANSWERS,   response.statistics[2]},
                {CORRECT_ANSWERS, response.statistics[3]}
            }
        }
    };

    return serializeGeneralResponse(ResponseCode::OK, j.dump());
}


buffer JsonResponseSerializer::serializeGeneralResponse(const ResponseCode type, const std::string_view json)
{
    // Directly constructing the buffer for efficiency
    return {std::from_range,
            // The first byte is the response code
            std::to_string(type) + \
            // Pushing the JSON's length to the buffer
            Helper::getPaddedNumber(json.length(), BYTES_RESERVED_FOR_MSG_LEN) + \
            // Pushing the actual message to the buffer
            json.data()
            };
}
