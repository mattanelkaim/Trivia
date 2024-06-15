#pragma once

#include "InvalidProtocolStructure.h"
#include "ServerDefinitions.h"
#include <cstddef> // size_t
#include <ctre.hpp>
#include <string>
#include <string_view>
#include <type_traits> //std::is_convertible
#include <WinSock2.h>

namespace Helper
{
    /*######################################
    ############ COMMUNICATION #############
    ######################################*/

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


    /*######################################
    ################ REGEX #################
    ######################################*/


    // Username: 1-16 characters, only letters, numbers, and underscores
    constexpr auto usernameMatcher = ctre::match<R"([a-zA-Z0-9_]{1,16})">;

    // Password: 5-20 characters, at least 1 uppercase, 1 lowercase, 1 number, and 1 special character
    constexpr auto passwordMatcher = ctre::match<R"(^(?=.*\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[!@#$%^&*()\-_=+|{};:/?.])([a-zA-Z0-9!@#$%^&*()\-_=+|{};:/?.]{5,20})$)">;

    // Email: RFC 5322
    constexpr auto emailMatcher = ctre::match<R"([a-zA-Z0-9._%+\-]+@[a-zA-Z0-9.\-]+\.[a-zA-Z]{2,})">;

    constexpr bool isUsernameValid(const std::string_view username) noexcept
    {
        return usernameMatcher(username);
    }

    constexpr bool isPasswordValid(const std::string_view password) noexcept
    {
        return passwordMatcher(password);
    }

    constexpr bool isEmailValid(const std::string_view email) noexcept
    {
        return emailMatcher(email);
    }


    /*######################################
    ################ OTHER #################
    ######################################*/


    constexpr std::string getPaddedNumber(const std::unsigned_integral auto num, const size_t digits) noexcept
    {
        // Return string after padding zeros if necessary
        const std::string numStr = std::to_string(num);
        return std::string(digits - numStr.size(), '0') + numStr;
    }

    /**
    * @brief Converts a string (implicit json-shit) OR another integral type to another integral type.
    * @tparam ReturnType The integral type to convert to.
    * @tparam T The type of the object to convert (string or another integral).
    * @param obj The object to convert (string or another integral).
    * @return The converted object (integral type).
    * @throws InvalidProtocolStructure (only relevant for string conversion)
    */
    template <std::integral ReturnType, typename T>
    constexpr ReturnType tryMakeIntegral(const T& obj) // noexcept(std::is_convertible<T, ReturnType>())
    {
        if constexpr (std::is_integral<T>())
            return static_cast<ReturnType>(obj);

        try
        {
            // Casting json library shit to an explicit string
            return static_cast<ReturnType>(std::stoll(static_cast<std::string>(obj)));
        }
        catch (...) // std::invalid_argument OR std::out_of_range
        {
            throw InvalidProtocolStructure{"Cannot convert " + static_cast<std::string>(obj) + " to integral type!"}; // Throwing a more specific exception
        }
    }

    std::string formatError(const std::string& functionName, const std::string& err);

}; // namespace Helper
