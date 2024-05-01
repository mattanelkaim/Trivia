#include "../json.hpp"
#include "../ServerDefenitions.h"
#include "JsonRequestDeserializer.h"
#include <stdexcept>
#include <string>

using json = nlohmann::json;

#include <iostream>
LoginRequest JsonResponseDeserializer::deserializeLoginResponse(const buffer& response)
{
    LoginRequest request;

    try
    {
        // Parse string msg to JSON
        const json j = json::parse(response);
        std::cout << j.dump() << '\n';

        // Store data from JSON on a LoginRequest struct
        j.at("username").get_to(request.username);
        j.at("password").get_to(request.password);
    }
    catch (const json::parse_error& e)
    {
        throw std::runtime_error("ERROR parsing json response at byte " + e.byte);
    }

    return request;
}

SignupRequest JsonResponseDeserializer::deserializeSignupResponse(const buffer& response)
{
    SignupRequest request;

    try
    {
        // Parse string msg to JSON
        const json j = json::parse(response);

        // Store data from JSON on a SignupRequest struct
        j.at("username").get_to(request.username);
        j.at("password").get_to(request.password);
        j.at("email").get_to(request.email);
    }
    catch (const json::parse_error& e)
    {
        throw std::runtime_error("ERROR parsing json response at byte " + e.byte);
    }
    
    return request;
}
