#include "../ServerDefenitions.h"
#include "Helper.h"
#include <iomanip>
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
    std::cout << "reading " << bytesNum << " bytes\n";
    const std::string data = getStringFromSocket(sc, bytesNum);

    std::cout << "received '" << data << "'\n";
    return stoi(data);
}

// return string after padding zeros if necessary
std::string Helper::getPaddedNumber(const size_t& num, const size_t& digits) noexcept
{
    std::ostringstream oStr;
    oStr << std::setw(static_cast<std::streamsize>(digits)) << std::setfill('0') << num;
    return oStr.str();
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
    return str;
}

// send data to socket
// this is private function
void Helper::sendData(SOCKET sc, const std::string& message)
{
    if (send(sc, message.c_str(), static_cast<int>(message.size()), 0) == INVALID_SOCKET) // flags = 0
        throw std::runtime_error("Error while sending message to client. Error: " + to_string(WSAGetLastError()));
}
