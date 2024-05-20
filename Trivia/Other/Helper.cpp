#include "Helper.h"
#include "InvalidProtocolStructure.h"
#include "ServerDefinitions.h"
#include "UnexpectedClientExit.h"
#include <cstddef> // size_t
#include <stdexcept>
#include <string>
#include <string_view>
#include <WinSock2.h>
#if SERVER_DEBUG
#include <iostream>
#endif

using std::to_string;

std::string Helper::getMessageFromSocket(const SOCKET sc)
{
    const int length = [sc] { // Using IIFE
        try
        {
            return getIntPartFromSocket(sc, BYTES_RESERVED_FOR_MSG_LEN);
        }
        catch (const std::invalid_argument&)
        {
            throw InvalidProtocolStructure("Invalid protocol structure: length");
        }
    }();

    return getStringFromSocket(sc, length);
}

int Helper::getCodeFromSocket(const SOCKET sc)
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
int Helper::getIntPartFromSocket(const SOCKET sc, const int bytesNum)
{
    return stoi(getStringFromSocket(sc, bytesNum));
}

// bytesNum is not unsigned to match recv parameter specification
std::string Helper::getStringFromSocket(const SOCKET sc, const int bytesNum)
{
    if (bytesNum <= 0) return "";

    const auto bufferSize = static_cast<size_t>(bytesNum);
    char* data = new char[bufferSize]; // Terminator is added later

    if (recv(sc, data, bytesNum, 0) == SOCKET_ERROR) // flags = 0
    {
        delete[] data;
        const int errCode = WSAGetLastError();
        if (errCode == CLIENT_CLOSED_UNEXPECTEDLY) [[unlikely]]
            throw UnexpectedClientExit(sc);
        else [[likely]]
            throw std::runtime_error("Error while receiving from socket: " + to_string(sc) + " | Error: " + to_string(errCode));
    }

    const std::string str(data, bufferSize); // Null terminator added automatically
    delete[] data;

#if SERVER_DEBUG
    std::cout << "[CLIENT] " << str << "\n";
#endif
    return str;
}

// send data to socket
// this is private function
void Helper::sendData(const SOCKET sc, const std::string_view message)
{
#if SERVER_DEBUG
    std::cout << "[SERVER] " << message << '\n';
#endif
    if (send(sc, message.data(), static_cast<int>(message.size()), 0) == SOCKET_ERROR) // flags = 0
        throw std::runtime_error("Error while sending message to client. Error: " + to_string(WSAGetLastError()));
}
