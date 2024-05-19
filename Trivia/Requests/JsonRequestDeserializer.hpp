#pragma once

#include "json.hpp"
#include "ServerDefinitions.h"
#include <concepts>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

class JsonRequestDeserializer final
{
public:
    JsonRequestDeserializer() = delete; // This ensures that this class is never instantiated

    template <std::derived_from<Request> T>
    static T deserializeRequest(const readonly_buffer requestBuffer)
    {
        T request{};

        try
        {
            // Parse string msg to JSON
            const json j = json::parse(requestBuffer);

            // Store data from JSON on a struct
            if constexpr (std::same_as<T, LoginRequest> || std::same_as<T, SignupRequest>)
            {
                j.at("username").get_to(request.username);
                j.at("password").get_to(request.password);

                if constexpr (std::same_as<T, SignupRequest>)
                {
                    j.at("email").get_to(request.email);
                }
            }
            else if constexpr (std::same_as<T, CreateRoomRequest>)
            {
                j.at("roomName").get_to(request.roomName);
                j.at("maxUsers").get_to(request.maxUsers);
                j.at("questionCount").get_to(request.questionCount);
                j.at("answerTimeout").get_to(request.answerTimeout);
            }
            else if constexpr (requires{ T::roomId; }) // Either GetPlayersInRoomRequest or JoinRoomRequest
            {
                j.at("roomId").get_to(request.roomId);
            }
            else
            {
                throw std::runtime_error("not supported yet");
            }
        }
        catch (const json::parse_error& e)
        {
            throw std::runtime_error("ERROR parsing JSON response at byte " + std::to_string(e.byte));
        }
        catch (const json::out_of_range& e)
        {
            throw std::runtime_error(std::string("ERROR parsing JSON: ") + e.what());
        }

        return request;
    }
};
