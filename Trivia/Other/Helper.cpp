#include "Helper.h"
#include "InvalidProtocolStructure.h"
#include "ServerDefinitions.h"
#include "UnexpectedClientExit.h"
#include <string>
#if defined(SERVER_DEBUG_ALL) || defined(SERVER_DEBUG_IN) || defined(SERVER_DEBUG_OUT)
#include <iostream>
#endif

using std::to_string;

std::string Helper::getMessageFromSocket(SOCKET sc)
{
    int length;
    try
    {
        length = getIntPartFromSocket(sc, BYTES_RESERVED_FOR_MSG_LEN);
    }
    catch (const std::invalid_argument&)
    {
        throw InvalidProtocolStructure("Invalid protocol structure: length");
    }

    return getStringFromSocket(sc, length);
}

int Helper::getCodeFromSocket(SOCKET sc)
{
    try
    {
        return getIntPartFromSocket(sc, BYTES_RESERVED_FOR_CODE);
    }
    catch (const std::invalid_argument&)
    {
        throw InvalidProtocolStructure("Invalid protocol structure: code");
    }
}

// receive data from socket according byteSize
// returns the data as int
int Helper::getIntPartFromSocket(SOCKET sc, const int bytesNum)
{
    return stoi(getStringFromSocket(sc, bytesNum));
}

// Return string after padding zeros if necessary
std::string Helper::getPaddedNumber(const uint32_t num, const size_t digits)
{
    // Cannot be constexpr nor noexcept cuz to_string is kaki
    const std::string numStr = std::to_string(num);
    return std::string(digits - numStr.size(), '0') + numStr;
}

// bytesNum is not unsigned to match recv parameter specification
std::string Helper::getStringFromSocket(SOCKET sc, const int bytesNum)
{
    if (bytesNum <= 0) return "";

    const size_t bufferSize = static_cast<size_t>(bytesNum);
    char* data = new char[bufferSize]; // Terminator is added later

    if (recv(sc, data, bytesNum, 0) == static_cast<int>(INVALID_SOCKET)) // flags = 0
    {
        delete[] data;
        const int errCode = WSAGetLastError();
        if (errCode == CLIENT_CLOSED_UNEXPECTEDLY)
            throw UnexpectedClientExit(sc);
        else
            throw std::runtime_error("Error while receiving from socket: " + to_string(sc) + " | Error: " + to_string(errCode));
    }

    const std::string str(data, bufferSize); // Null terminator added automatically
    delete[] data;

#if defined(SERVER_DEBUG_ALL) || defined(SERVER_DEBUG_IN)
    std::cout << "[CLIENT] " << str << "\n";
#endif
    return str;
}

// send data to socket
// this is private function
void Helper::sendData(SOCKET sc, std::string_view message)
{
#if defined(SERVER_DEBUG_ALL) || defined(SERVER_DEBUG_OUT)
    std::cout << "[SERVER] " << message << '\n';
#endif
    if (send(sc, message.data(), static_cast<int>(message.size()), 0) == static_cast<int>(INVALID_SOCKET)) // flags = 0
        throw std::runtime_error("Error while sending message to client. Error: " + to_string(WSAGetLastError()));
}
