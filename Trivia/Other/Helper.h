#pragma once

#include <concepts>
#include <cstddef> // size_t
#include <string>
#include <string_view>
#include <WinSock2.h>

namespace Helper
{
    std::string getStringFromSocket(SOCKET sc, int bytesNum);
    int getIntPartFromSocket(SOCKET sc, int bytesNum);
    std::string getMessageFromSocket(SOCKET sc); // Helper function
    int getCodeFromSocket(SOCKET sc); // Helper function
    void sendData(SOCKET sc, std::string_view message);

    std::string getPaddedNumber(const std::unsigned_integral auto num, const size_t digits)
    {
        // Return string after padding zeros if necessary
        const std::string numStr = std::to_string(num);
        return std::string(digits - numStr.size(), '0') + numStr;
    }
}; // namespace Helper
