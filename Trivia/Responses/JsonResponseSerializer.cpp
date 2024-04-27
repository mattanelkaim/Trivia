#include "../Infrastructure/Helper.h"
#include "JsonResponseSerializer.h"
#include <string>


buffer JsonResponseSerializer::serializeErrorResponse(const ErrorResponse& response)
{
    return serializeGeneralResponse(messageType::RESPONSE, ERROR_MSG_JSON.data());
}

buffer JsonResponseSerializer::serializeLoginResponse(const LoginResponse& response)
{
    return serializeGeneralResponse(messageType::RESPONSE, "{status: 1}");
}

buffer JsonResponseSerializer::serializeSignupResponse(const SignupResponse& response)
{
    return serializeGeneralResponse(messageType::RESPONSE, "{status: 1}");
}

buffer JsonResponseSerializer::serializeGeneralResponse(const messageType& type, const std::string& json)
{
    buffer buff;

    // The first byte is the response code
    buff.append_range(std::to_string(type));

    // Pushing the JSON's length to the buffer
    buff.append_range(Helper::getPaddedNumber(json.length(), BYTES_RESERVED_FOR_MSG_LEN));

    // Pushing the actual message to the buffer
    buff.append_range(json);

    return buff;
}
