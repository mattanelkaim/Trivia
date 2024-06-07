#pragma once

#include "ServerDefinitions.h"
#include "InvalidProtocolStructure.h"
#include <type_traits> //std::is_convertible
#include <cstddef> // size_t
#include <string>
#include <string_view>
#include <WinSock2.h>

namespace Helper
{
    /**
    * @throws InvalidProtocolStructure  
    */
    template <std::integral ReturnType, typename T>
    ReturnType tryMakeIntegral(const T& obj)
    {
        if (std::is_convertible<T, ReturnType>()) 
            return obj;

        try
        {
            return static_cast<ReturnType>(std::stoll(static_cast<std::string>(obj)));
        }
        catch (...)
        {
            throw InvalidProtocolStructure(); // throwing a more specific exception
        }
    }

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
    void sendData(SOCKET sc, const buffer& message);

    /**
     * @throws std::runtime_error
     */
    void sendData(SOCKET sc, std::string_view message);

    std::string getPaddedNumber(const std::unsigned_integral auto num, const size_t digits) noexcept
    {
        // Return string after padding zeros if necessary
        const std::string numStr = std::to_string(num);
        return std::string(digits - numStr.size(), '0') + numStr;
    }
}; // namespace Helper
