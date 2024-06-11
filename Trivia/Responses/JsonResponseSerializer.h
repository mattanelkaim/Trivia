#pragma once

#include "json.hpp"
#include "ServerDefinitions.h"
#include <string_view>

namespace JsonResponseSerializer
{
    using json = nlohmann::json;

    // Helper function
    buffer serializeGeneralResponse(ResponseCode type, std::string_view json) noexcept;

    buffer serializeResponse(const ErrorResponse& response) noexcept;
    buffer serializeResponse(const GetRoomsResponse& response) noexcept;
    buffer serializeResponse(const GetPlayersInRoomResponse& response) noexcept;
    buffer serializeResponse(const GetHighScoreResponse& response) noexcept;
    buffer serializeResponse(const GetPersonalStatsResponse& response) noexcept;         
    buffer serializeResponse(const GetRoomStateResponse& response) noexcept;       

    /**
     * Serializes a general response that isn't covered in other overloads of this function,
     * with a status field.
     *
     * @param response The response object to be serialized.
     * @return The serialized response as a buffer.
     */
    buffer serializeResponse(const auto& response) noexcept requires requires { response.status;  } // NOLINT
    {
        const json j{{JsonFields::STATUS, response.status}};
        return serializeGeneralResponse(ResponseCode::OK, j.dump());
    }
}; // namespace JsonResponseSerializer
