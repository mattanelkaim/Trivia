#include "Helper.h"
#include "json.hpp"
#include "JsonResponseSerializer.h"
#include <string>

using json = nlohmann::json;

buffer JsonResponseSerializer::serializeErrorResponse(const ErrorResponse& response)
{
    json j;
    j[JsonFields::MESSAGE_FIELD] = response.message;
    return serializeGeneralResponse(messageType::RESPONSE, j.dump());
}

buffer JsonResponseSerializer::serializeLoginResponse(LoginResponse response)
{
    json j;
    j[JsonFields::STATUS_FIELD] = response.status;
    return serializeGeneralResponse(messageType::RESPONSE, j.dump());
}

buffer JsonResponseSerializer::serializeSignupResponse(SignupResponse response)
{
    json j;
    j[JsonFields::STATUS_FIELD] = response.status;
    return serializeGeneralResponse(messageType::RESPONSE, j.dump());
}

buffer JsonResponseSerializer::serializeGeneralResponse(messageType type, std::string_view json)
{
    buffer buff;

    // The first byte is the response code
    buff.append_range(std::to_string(type));

    // Pushing the JSON's length to the buffer
    buff.append_range(Helper::getPaddedNumber(static_cast<uint32_t>(json.length()), BYTES_RESERVED_FOR_MSG_LEN));

    // Pushing the actual message to the buffer
    buff.append_range(json);

    return buff;
}
