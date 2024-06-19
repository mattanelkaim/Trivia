#include "Helper.h"
#include "InvalidProtocolStructure.h"
#include "ServerDefinitions.h"
#include "UnexpectedClientExit.h"
#include <cstddef> // size_t
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <winerror.h> // for switch-case
#include <WinSock2.h>
#if PRINT_IO
#include <iostream>
#endif

using std::to_string;

std::string Helper::formatError(const std::string& functionName, const std::string& err) noexcept
{
    if constexpr (EXTENDED_ERRORS)
    {
        return "{ERROR | FUNCTION=" + functionName + \
            " | THREAD=" + to_string(std::this_thread::get_id()._Get_underlying_id()) + \
            " | MESSAGE=" + err + "}";
    }
    else
        return err;
}

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

int Helper::getIntPartFromSocket(const SOCKET sc, const int bytesNum)
{
    try
    {
        return std::stoi(getStringFromSocket(sc, bytesNum));
    }
    catch (const std::out_of_range&) // Might be thrown from stoi
    {
        throw InvalidProtocolStructure("Invalid protocol structure: expected int");
    }
}

std::string Helper::getStringFromSocket(const SOCKET sc, const int bytesNum)
{
    if (bytesNum <= 0) return "";

    const auto bufferSize = static_cast<size_t>(bytesNum);
    char* data = new char[bufferSize]; // Terminator is added later

    if (recv(sc, data, bytesNum, 0) == SOCKET_ERROR) // flags = 0
    {
        delete[] data;
        const int errCode = WSAGetLastError();
        switch (errCode)
        {
        case WSAECONNRESET: // Client closed unexpectedly
            throw UnexpectedClientExit(sc);
        case WSAECONNABORTED:
            throw ABORT_FLAG::ABORT; // Abort the handling thread
        [[likely]] default:
            throw std::runtime_error("Error while receiving from socket: " + to_string(sc) + " | Error: " + to_string(errCode));
        }
    }

    const std::string str(data, bufferSize); // Null terminator added automatically
    delete[] data;

#if PRINT_IO
    std::cout << "[CLIENT (" << sc << ")] " << str << "\n";
#endif
    return str;
}

void Helper::sendData(const SOCKET sc, const buffer& message)
{
    sendData(sc, std::string_view(reinterpret_cast<const char*>(message.data()), message.size()));
}

void Helper::sendData(const SOCKET sc, const std::string_view message)
{
#if PRINT_IO
    std::cout << "[SERVER -> " << sc << "] " << message << '\n';
#endif
    if (send(sc, message.data(), static_cast<int>(message.size()), 0) == SOCKET_ERROR) // flags = 0
        throw std::runtime_error("Error while sending message to client. Error: " + to_string(WSAGetLastError()));
}
