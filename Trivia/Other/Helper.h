#pragma once

#include "ServerDefinitions.h"
#include <concepts>
#include <cstddef> // size_t
#include <string>
#include <string_view>
#include <WinSock2.h>

namespace Helper
{
    /**
    * @throws InvalidProtocolStructure
    * @throws UnexpectedClientExit
    * @throws std::runtime_error
    */
    std::string getMessageFromSocket(SOCKET sc); // Helper function

    /**
    * @throws InvalidProtocolStructure
    * @throws UnexpectedClientExit
    * @throws std::runtime_error
    */
    int getCodeFromSocket(SOCKET sc); // Helper function

    /**
    * @throws InvalidProtocolStructure
    * @throws UnexpectedClientExit
    * @throws std::runtime_error
    * @throws std::invalid_argument // stoi
    */
    int getIntPartFromSocket(SOCKET sc, int bytesNum);

    /**
    * @param bytesNum is not unsigned to match recv parameter specification
    * @throws UnexpectedClientExit
    * @throws std::runtime_error
    */
    std::string getStringFromSocket(SOCKET sc, int bytesNum);

    /**
     * @brief Convenience wrapper
     * @throws std::runtime_error
     */
    void sendData(SOCKET sc, buffer message);

    /**
     * @throws std::runtime_error
     */
    void sendData(SOCKET sc, std::string_view message);

    std::string getPaddedNumber(const std::unsigned_integral auto num, const size_t digits)
    {
        // Return string after padding zeros if necessary
        const std::string numStr = std::to_string(num);
        return std::string(digits - numStr.size(), '0') + numStr;
    }
}; // namespace Helper
