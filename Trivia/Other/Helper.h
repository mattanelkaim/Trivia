#pragma once

#include <cstddef> // size_t
#include <cstdint>
#include <string>
#include <string_view>
#include <WinSock2.h>

class Helper
{
public:
    static std::string getStringFromSocket(SOCKET sc, int bytesNum);
    static int getIntPartFromSocket(SOCKET sc, int bytesNum);
    static std::string getMessageFromSocket(SOCKET sc); // Helper function
    static int getCodeFromSocket(SOCKET sc); // Helper function
    static void sendData(SOCKET sc, std::string_view message);
    static std::string getPaddedNumber(uint32_t num, size_t digits);
};
