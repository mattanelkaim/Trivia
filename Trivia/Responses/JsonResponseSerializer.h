#pragma once

#include "json.hpp"
#include "ServerDefinitions.h"
#include <string_view>

using json = nlohmann::json;

namespace JsonResponseSerializer
{
    // Helper function
    buffer serializeGeneralResponse(messageType type, std::string_view json);

    buffer serializeResponse(const ErrorResponse& response);
    buffer serializeResponse(const GetRoomsResponse& response);
    buffer serializeResponse(const GetPlayersInRoomResponse& response);
    buffer serializeResponse(const GetHighScoreResponse& response);
    buffer serializeResponse(const GetPersonalStatsResponse& response);

    template <typename T>
    buffer serializeResponse(const T& response)    
    {
        json j;
        j[JsonFields::STATUS] = response.status;
        return serializeGeneralResponse(messageType::RESPONSE, j.dump());
    }
}; // namespace JsonResponseSerializer
