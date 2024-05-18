#pragma once

#include <concepts>
#include <cstddef> // size_t
#include <cstdint>
#include <string>
#include <string_view>
#include <WinSock2.h>

class Helper final
{
public:
    Helper() = delete;

    static std::string getStringFromSocket(SOCKET sc, int bytesNum);
    static int getIntPartFromSocket(SOCKET sc, int bytesNum);
    static std::string getMessageFromSocket(SOCKET sc); // Helper function
    static int getCodeFromSocket(SOCKET sc); // Helper function
    static void sendData(SOCKET sc, std::string_view message);

    static std::string getPaddedNumber(const std::unsigned_integral auto num, const size_t digits) // Cannot be constexpr nor noexcept cuz to_string is kaki
    {
        // Return string after padding zeros if necessary
        const std::string numStr = std::to_string(num);
        return std::string(digits - numStr.size(), '0') + numStr;
    }
};
