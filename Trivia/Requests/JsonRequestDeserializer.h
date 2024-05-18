#pragma once

#include "ServerDefinitions.h"

class JsonResponseDeserializer
{
public:
    JsonResponseDeserializer() = delete; // This ensures that this class is never instantiated

    template <typename T>
    static T deserializeRequest(const readonly_buffer requestBuffer)
    {
        T request;

        try {
            // Parse string msg to JSON
            const json j = json::parse(requestBuffer);

            // Store data from JSON on a struct
            if constexpr (std::is_same_v<T, LoginRequest> || std::is_same_v<T, SignupRequest) 
            {
                j.at("username").get_to(request.username);
                j.at("password").get_to(request.password);

                if constexpr (std::is_same_v<T, Signup>)
                {
                    j.at("email").get_to(request.email);
                }
            }
            else if constexpr (std::is_same_v<T, GetPlayersInRoomRequest> || std::is_same_v<T, JoinRoomRequest>)
            {
                j.at("roomId").get_to(request.roomId)
            }
            else if constexpr (std::is_same_v<T, CreateRoomRequest>) /* it is OK to use "else if" in this case instead of just "else" because this whole if - else block is precompiled,
                                                                        also it is more readable like this */
            {
                j.at("roomName").get_to(request.roomName);
                j.at("maxUsers").get_to(request.maxUsers);
                j.at("questionCount").get_to(request.questionCount);
                j.at("answerTimeout").get_to(request.answerTimeout);
            }
        }
        catch (const json::parse_error& e) {
            throw std::runtime_error("ERROR parsing JSON response at byte " + std::to_string(e.byte));
        }
        catch (const json::out_of_range& e) {
            throw std::runtime_error(std::string("ERROR parsing JSON: ") + e.what());
        }

        return request;
    }
};
