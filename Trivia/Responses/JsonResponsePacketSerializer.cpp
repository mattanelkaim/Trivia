#include "../Infrastructure/Helper.h"
#include "JsonResponsePacketSerializer.h"


buffer JsonResponsePacketSerializer::serializeErrorResponse(const ErrorResponse& response)
{
    buffer buff;

    buff.push_back(messageType::RESPONSE); // The first byte is the response code

    // Pushing the JSON's length to the buffer
    buff.append_range(Helper::getPaddedNumber(ERROR_MSG_JSON.length(), BYTES_RESERVED_FOR_MSG_LEN));

    // Pushing the actual message to the buffer
    buff.append_range(ERROR_MSG_JSON);
    
    return buff;
}

buffer JsonResponsePacketSerializer::serializeLoginResponse(const LoginResponse& response)
{
    buffer buff;
    const std::string_view json = "{status: 1}";

    buff.push_back(messageType::RESPONSE); // The first byte is the response code

    // Pushing the JSON's length to the buffer
    buff.append_range(Helper::getPaddedNumber(json.length(), BYTES_RESERVED_FOR_MSG_LEN));

    // Pushing the actual message to the buffer
    buff.append_range(json);

    return buff;
}

buffer JsonResponsePacketSerializer::serializeSignupResponse(const SignupResponse& response)
{
    buffer buff;
    const std::string_view json = "{status: 1}";

    buff.push_back(messageType::RESPONSE); // The first byte is the response code

    // Pushing the JSON's length to the buffer
    buff.append_range(Helper::getPaddedNumber(json.length(), BYTES_RESERVED_FOR_MSG_LEN));

    // Pushing the actual message to the buffer
    buff.append_range(json);

    return buff;
}
