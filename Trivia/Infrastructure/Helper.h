#pragma once

#include <cstdint>
#include <string>
#include <WinSock2.h>

class Helper
{
public:
    static int getMessageTypeCode(SOCKET sc);
    static int getIntPartFromSocket(SOCKET sc, const uint32_t& bytesNum);
    static std::string getStringPartFromSocket(SOCKET sc, const uint32_t& bytesNum);
    static void sendData(SOCKET sc, const std::string& message);
    static std::string getPaddedNumber(const size_t& num, const size_t& digits) noexcept;

private:
    static char* getPartFromSocket(SOCKET sc, const uint32_t& bytesNum, const int& flags = 0);
};
