#pragma once

#include "json.hpp"
#include "ServerDefinitions.h"

using json = nlohmann::json;

class JsonResponseSerializer
{
public:
    JsonResponseSerializer() = delete; // This ensures that this class is never instantiated

    static buffer serializeResponse(const ErrorResponse& response);
    static buffer serializeResponse(const GetRoomsResponse& response);
    static buffer serializeResponse(const GetPlayersInRoomResponse& response);
    static buffer serializeResponse(const GetHighScoreResponse& response);
    static buffer serializeResponse(const GetPersonalStatsResponse& response);

    template <typename T>
    static buffer serializeResponse(const T& response)    
    {
        json j;
        j[JsonFields::STATUS] = response.status;
        return serializeGeneralResponse(messageType::RESPONSE, j.dump());
    }

private:
    static buffer serializeGeneralResponse(const messageType type, const std::string_view json); // Internal helper function
};