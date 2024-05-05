#pragma once

#include <cstdint>
#include <string>
#include <WinSock2.h>

class Helper
{
public:
    static std::string getStringFromSocket(SOCKET sc, const int bytesNum);
    static int getIntPartFromSocket(SOCKET sc, const int bytesNum);
    static std::string getMessageFromSocket(SOCKET sc); // Helper function
    static int getCodeFromSocket(SOCKET sc); // Helper function
    static void sendData(SOCKET sc, const std::string_view message);
    static std::string getPaddedNumber(const uint32_t num, const size_t digits);
};
