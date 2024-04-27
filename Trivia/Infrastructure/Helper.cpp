#include "Helper.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using std::to_string;

// receives the type code of the message from socket (3 bytes)
// and returns the code. if no message found in the socket returns 0 (which means the client disconnected)
int Helper::getMessageTypeCode(SOCKET sc)
{
    return getIntPartFromSocket(sc, 3);
}

// receive data from socket according byteSize
// returns the data as int
int Helper::getIntPartFromSocket(SOCKET sc, const uint32_t& bytesNum)
{
    return stoi(getStringPartFromSocket(sc, bytesNum));
}

// receive data from socket according byteSize
// returns the data as string
std::string Helper::getStringPartFromSocket(SOCKET sc, const uint32_t& bytesNum)
{
    const char* s = getPartFromSocket(sc, bytesNum, 0);
    const std::string str(s);
    delete[] s;
    return str;
}

// return string after padding zeros if necessary
std::string Helper::getPaddedNumber(const size_t& num, const size_t& digits) noexcept
{
    std::ostringstream oStr;
    oStr << std::setw((std::streamsize)digits) << std::setfill('0') << num;
    return oStr.str();
}

// Must delete returned pointer!
char* Helper::getPartFromSocket(SOCKET sc, const uint32_t& bytesNum, const int& flags)
{
    if (bytesNum <= 0) return const_cast<char*>("");

    char* data = new char[bytesNum + 1]; // Allocate data for msg with terminator

    if (recv(sc, data, static_cast<int>(bytesNum), flags) == INVALID_SOCKET)
    {
        std::ostringstream oStr;
        oStr << "Error while receiving from socket: " << sc << " | Error: " << WSAGetLastError();
        delete[] data;
        throw std::runtime_error(oStr.str());
    }

    data[bytesNum] = 0; // Terminator
    return data;
}

// send data to socket
// this is private function
void Helper::sendData(SOCKET sc, const std::string& message)
{
    if (send(sc, message.c_str(), static_cast<int>(message.size()), 0) == INVALID_SOCKET)
        throw std::runtime_error("Error while sending message to client. Error: " + to_string(WSAGetLastError()));
}
