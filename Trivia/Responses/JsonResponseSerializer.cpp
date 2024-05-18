#include "Helper.h"
#include "JsonResponseSerializer.h"
#include "ServerDefinitions.h"
#include <cstddef> // size_t
#include <cstdint>
#include <string>
#include <string_view>

buffer JsonResponseSerializer::serializeResponse(const ErrorResponse& response)
{
    json j;
    j[JsonFields::MESSAGE] = response.message;
    return serializeGeneralResponse(messageType::RESPONSE, j.dump());
}

buffer JsonResponseSerializer::serializeResponse(const GetRoomsResponse& response)
{
    // Join all string fields with a delimiter
    std::string names;
    for (const RoomData& room : response.rooms)
        names += room.name + ", ";
    names.resize(names.size() - 2); // Delete last 2 chars (", ")

    json j;
    j[JsonFields::ROOMS] = names;
    return serializeGeneralResponse(messageType::RESPONSE, j.dump());
}

buffer JsonResponseSerializer::serializeResponse(const GetPlayersInRoomResponse& response)
{
    // Join all strings with a delimiter
    std::string names;
    for (const std::string& room : response.players)
        names += room + ", ";
    names.resize(names.size() - 2); // Delete last 2 chars (", ")

    json j;
    j[JsonFields::PLAYERS_IN_ROOM] = names;
    return serializeGeneralResponse(messageType::RESPONSE, j.dump());
}

buffer JsonResponseSerializer::serializeResponse(const GetHighScoreResponse& response)
{
    json j;
    
    // Fill json with inner fields, or with "None" if non-existent
    for (size_t i = 0; i < NUM_TOP_SCORES; ++i)
        j[JsonFields::HIGH_SCORES][i + 1] = (i < response.statistics.size()) ? response.statistics[i] : "None";

    return serializeGeneralResponse(messageType::RESPONSE, j.dump());
}

buffer JsonResponseSerializer::serializeResponse(const GetPersonalStatsResponse& response)
{
    json j;

    // Sub-fields that construct the "userStatistics" outer field
    using namespace JsonFields;
    using namespace JsonFields::UserStats;
    j[STATISTICS][SCORE] = response.statistics[0];
    j[STATISTICS][TOTAL_GAMES] = response.statistics[1];
    j[STATISTICS][TOTAL_ANSWERS] = response.statistics[2];
    j[STATISTICS][CORRECT_ANSWERS] = response.statistics[3];

    return serializeGeneralResponse(messageType::RESPONSE, j.dump());
}


buffer JsonResponseSerializer::serializeGeneralResponse(const messageType type, const std::string_view json)
{
    buffer buff;

    // The first byte is the response code
    buff.append_range(std::to_string(type));

    // Pushing the JSON's length to the buffer
    buff.append_range(Helper::getPaddedNumber(json.length(), BYTES_RESERVED_FOR_MSG_LEN));

    // Pushing the actual message to the buffer
    buff.append_range(json);

    return buff;
}
