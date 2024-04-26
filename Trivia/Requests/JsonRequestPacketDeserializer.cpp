#include "../Infrastructure/Helper.h"
#include "../json.hpp"
#include "../ServerDefenitions.h"
#include "JsonRequestPacketDeserializer.h"
#include <stdexcept>

using json = nlohmann::json;

LoginRequest JsonResponsePacketDeserializer::deserializeLoginResponse(const buffer& response)
{
    if (response.size() < JSON_OFFSET + 2) // +2 curly braces
        throw std::runtime_error("Response does not match protocol structure");

    // Parse string msg to JSON
    const std::string jsonMsg(response.cbegin() + JSON_OFFSET, response.cend());
    json j = json::parse(jsonMsg);

    // Store data from JSON on a LoginRequest struct
    LoginRequest request;
    j.at("username").get_to(request.username);
    j.at("password").get_to(request.password);

    return request;
}

SignupRequest JsonResponsePacketDeserializer::deserializeSignupResponse(const buffer& response)
{
    if (response.size() < JSON_OFFSET + 2) // +2 curly braces
        throw std::runtime_error("Response does not match protocol structure");

    // Parse string msg to JSON
    const std::string jsonMsg(response.cbegin() + JSON_OFFSET, response.cend());
    json j = json::parse(jsonMsg);

    // Store data from JSON on a SignupRequest struct
    SignupRequest request;
    j.at("username").get_to(request.username);
    j.at("password").get_to(request.password);
    j.at("email").get_to(request.email);

    return request;
}
