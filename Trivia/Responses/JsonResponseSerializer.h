#pragma once

#include "ServerDefinitions.h"

class JsonResponseSerializer
{
public:
    JsonResponseSerializer() = delete; // This ensures that this class is never instantiated

    static buffer serializeResponse(const ErrorResponse& response);
    static buffer serializeResponse(const LoginResponse response);
    static buffer serializeResponse(const SignupResponse response);
    static buffer serializeResponse(const LogoutResponse response);
    static buffer serializeResponse(const GetRoomsResponse& response);
    static buffer serializeResponse(const GetPlayersInRoomResponse& response);
    static buffer serializeResponse(const JoinRoomResponse response);
    static buffer serializeResponse(const CreateRoomResponse response);
    static buffer serializeResponse(const GetHighScoreResponse& response);
    static buffer serializeResponse(const GetPersonalStatsResponse& response);

private:
    static buffer serializeGeneralResponse(const messageType type, const std::string_view json); // Internal helper function
};
