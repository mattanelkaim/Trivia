#include "../ServerDefenitions.h"
#include "Helper.h"
#include <iostream>
#include <sstream>
#include <string>

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
        throw std::runtime_error("Invalid protocol structure: length"); // TODO(mattan) protocol_error
    }

    return getStringFromSocket(sc, static_cast<uint32_t>(length));
}

int Helper::getCodeFromSocket(SOCKET sc)
{
    try
    {
        return getIntPartFromSocket(sc, BYTES_RESERVED_FOR_CODE);
    }
    catch (const std::invalid_argument&)
    {
        throw std::runtime_error("Invalid protocol structure: code"); // TODO(mattan) protocol_error
    }
}

// receive data from socket according byteSize
// returns the data as int
int Helper::getIntPartFromSocket(SOCKET sc, const uint32_t& bytesNum)
{
    return stoi(getStringFromSocket(sc, bytesNum));
}

// return string after padding zeros if necessary
std::string Helper::getPaddedNumber(const size_t& num, const size_t& digits)
{
    const std::string numStr = std::to_string(num);
    return std::string(digits - numStr.size(), '0') + numStr;
}

// Must delete returned pointer!
std::string Helper::getStringFromSocket(SOCKET sc, const uint32_t& bytesNum)
{
    if (bytesNum <= 0) return const_cast<char*>("");

    char* data = new char[bytesNum + 1]; // Allocate data for msg with terminator

    if (recv(sc, data, static_cast<int>(bytesNum), 0) == INVALID_SOCKET) // flags = 0
    {
        std::ostringstream oStr;
        oStr << "Error while receiving from socket: " << sc << " | Error: " << WSAGetLastError();
        delete[] data;
        throw std::runtime_error(oStr.str());
    }

    data[bytesNum] = 0; // Terminator
    const std::string str(data);
    delete[] data;

#if defined(SERVER_DEBUG_ALL) || defined(SERVER_DEBUG_IN)
    std::cout << "[CLIENT] " << str << '\n';
#endif
    return str;
}

// send data to socket
// this is private function
void Helper::sendData(SOCKET sc, const std::string& message)
{
#if defined(SERVER_DEBUG_ALL) || defined(SERVER_DEBUG_OUT)
    std::cout << "[SERVER] " << message << '\n';
#endif
    if (send(sc, message.c_str(), static_cast<int>(message.size()), 0) == INVALID_SOCKET) // flags = 0
        throw std::runtime_error("Error while sending message to client. Error: " + to_string(WSAGetLastError()));
}
