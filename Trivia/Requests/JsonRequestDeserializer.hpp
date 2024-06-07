#pragma once

#include "Helper.h"
#include "InvalidProtocolStructure.h"
#include "json.hpp"
#include "ServerDefinitions.h"
#include <cstdint> // uint16_t
#include <string>

namespace JsonRequestDeserializer
{
    using json = nlohmann::json;

    /**
     * Deserialize a JSON request buffer into a specific request type.
     * @tparam RequestType The type of the request to deserialize.
     * @param requestBuffer The buffer containing the JSON request.
     * @return The deserialized request object.
     * @throws InvalidProtocolStructure
     */
    template <std::derived_from<Request> RequestType>
    RequestType deserializeRequest(const readonly_buffer requestBuffer)
    {
        try
        {
            // Parse string msg to JSON
            const json j = json::parse(requestBuffer);

            // Store data from JSON on a struct
            if constexpr (std::same_as<RequestType, LoginRequest>)
            {
                return RequestType{
                    .username = j.at("username"),
                    .password = j.at("password")
                };
            }
            else if constexpr (std::same_as<RequestType, SignupRequest>)
            {
                return RequestType{
                    .username = j.at("username"),
                    .password = j.at("password"),
                    .email    = j.at("email")
                };
            }
            else if constexpr (std::same_as<RequestType, CreateRoomRequest>)
            {
                return RequestType{
                    .roomName = j.at("roomName"),
                    .maxUsers = Helper::tryMakeIntegral<uint16_t>(j.at("maxUsers")),
                    .questionCount = Helper::tryMakeIntegral<uint16_t>(j.at("questionCount")),
                    .answerTimeout = Helper::tryMakeIntegral<uint32_t>(j.at("answerTimeout"))
                };
            }
            else if constexpr (requires{ RequestType::roomId; }) // Either GetPlayersInRoomRequest or JoinRoomRequest
            {
                return RequestType{.roomId = Helper::tryMakeIntegral<uint16_t>(j.at("roomId"))};
            }
            else
            {
                throw InvalidProtocolStructure("not supported yet");
            }
        }
        catch (const json::parse_error& e)
        {
            throw InvalidProtocolStructure("ERROR parsing JSON response at byte " + std::to_string(e.byte));
        }
        catch (const json::out_of_range& e)
        {
            throw InvalidProtocolStructure(std::string("ERROR parsing JSON: ") + e.what());
        }
    }
}; // namespace JsonRequestDeserializer
