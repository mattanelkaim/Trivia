#pragma once

#include "json.hpp"
#include "ServerDefinitions.h"
#include <string_view>

namespace JsonResponseSerializer
{
    using json = nlohmann::json;

    // Helper function
    buffer serializeGeneralResponse(messageType type, std::string_view json);

    buffer serializeResponse(const ErrorResponse& response);
    buffer serializeResponse(const GetRoomsResponse& response);
    buffer serializeResponse(const GetPlayersInRoomResponse& response);
    buffer serializeResponse(const GetHighScoreResponse& response);
    buffer serializeResponse(const GetPersonalStatsResponse& response);

    /**
     * Serializes a general response that isn't covered in other overloads of this function,
     * with a status field.
     *
     * @param response The response object to be serialized.
     * @return The serialized response as a buffer.
     */
    buffer serializeResponse(const auto& response) requires requires { response.status;  }
    {
        const json j{{JsonFields::STATUS, response.status}};
        return serializeGeneralResponse(messageType::RESPONSE, j.dump());
    }
}; // namespace JsonResponseSerializer
