#include "../json.hpp"
#include "../ServerDefenitions.h"
#include "JsonRequestDeserializer.h"
#include <stdexcept>
#include <string>

using json = nlohmann::json;

LoginRequest JsonResponseDeserializer::deserializeLoginResponse(const buffer& response)
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

SignupRequest JsonResponseDeserializer::deserializeSignupResponse(const buffer& response)
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
