#pragma once

#include "InvalidProtocolStructure.h"
#include "json.hpp"
#include "ServerDefinitions.h"
#include <concepts>
#include <stdexcept>
#include <string>

namespace JsonRequestDeserializer
{
    using json = nlohmann::json;

    /**
     * Deserialize a JSON request buffer into a specific request type.
     * 
     * @tparam T The type of the request to deserialize.
     * @param requestBuffer The buffer containing the JSON request.
     * @return The deserialized request object.
     * @throws InvalidProtocolStructure
     */
    template <std::derived_from<Request> T>
    T deserializeRequest(const readonly_buffer requestBuffer)
    {
        try
        {
            // Parse string msg to JSON
            const json j = json::parse(requestBuffer);

            // Store data from JSON on a struct
            if constexpr (std::same_as<T, LoginRequest>)
            {
                return T{
                    .username = j.at("username"),
                    .password = j.at("password")
                };
            }
            else if constexpr (std::same_as<T, SignupRequest>)
            {
                return T{
                    .username = j.at("username"),
                    .password = j.at("password"),
                    .email    = j.at("email")
                };
            }
            else if constexpr (std::same_as<T, CreateRoomRequest>)
            {
                return T{
                    .roomName      = j.at("roomName"),
                    .maxUsers      = j.at("maxUsers"),
                    .questionCount = j.at("questionCount"),
                    .answerTimeout = j.at("answerTimeout")
                };
            }
            else if constexpr (requires{ T::roomId; }) // Either GetPlayersInRoomRequest or JoinRoomRequest
            {
                return T{.roomId = j.at("roomId")};
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
