#include "JsonResponsePacketSerializer.h"
#include "../Infrastructure/Helper.h"

buffer JsonResponsePacketSerializer::serializeErrorResponse(const ErrorResponse& response)
{
    buffer buff(0);
    std::string json = ERROR_MESSAGE_JSON;

    buff.push_back(messageType::RESPONSE); // the first byte is the response code

    // pushing the json's length to the buffer
    for (const byte& b : Helper::getPaddedNumber(ERROR_MESSAGE_JSON_LENGTH, NUM_BYTES_RESERVED_FOR_MESSAGE_LENGTH))
        buff.push_back(b);

    // pushing the actual message to the buffer
    for (const byte& b : json)
        buff.push_back(b);
    
    return buff;
}       

buffer JsonResponsePacketSerializer::serializeLoginResponse(const LoginResponse& response)
{
    buffer buff(0);
    std::string json = "{status: 1}";

    buff.push_back(messageType::RESPONSE); // the first byte is the response code

    // pushing the json's length to the buffer
    for (const byte& b : Helper::getPaddedNumber(json.length(), NUM_BYTES_RESERVED_FOR_MESSAGE_LENGTH))
        buff.push_back(b);

    // pushing the actual message to the buffer
    for (const byte& b : json)
        buff.push_back(b);

    return buff;
}

buffer JsonResponsePacketSerializer::serializeSignupResponse(const SignupResponse& response)
{
    buffer buff(0);
    std::string json = "{status: 1}";

    buff.push_back(messageType::RESPONSE); // the first byte is the response code

    // pushing the json's length to the buffer
    for (const byte& b : Helper::getPaddedNumber(json.length(), NUM_BYTES_RESERVED_FOR_MESSAGE_LENGTH))
        buff.push_back(b);

    // pushing the actual message to the buffer
    for (const byte& b : json)
        buff.push_back(b);

    return buff;
}
