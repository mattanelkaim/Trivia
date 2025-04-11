#pragma once

#include "InvalidProtocolStructure.h"
#include "ServerDefinitions.h"
#include <concepts>
#include <cstddef> // size_t
#include <cstdint>
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
    constexpr auto isUsernameValid = ctre::match<R"([a-zA-Z0-9_]{1,16})">;

    // Password: 5-20 characters, at least 1 uppercase, 1 lowercase, 1 number, and 1 special character
    constexpr auto isPasswordValid = ctre::match<R"(^(?=.*\d)(?=.*[a-z])(?=.*[A-Z])(?=.*[!@#$%^&*()\-_=+|{};:/?.])([a-zA-Z0-9!@#$%^&*()\-_=+|{};:/?.]{5,20})$)">;

    // Email: RFC 5322
    constexpr auto isEmailValid = ctre::match<R"([a-zA-Z0-9._%+\-]+@[a-zA-Z0-9.\-]+\.[a-zA-Z]{2,})">;
    
    // Phone number: 10 digits
    constexpr auto isPhoneValid = ctre::match<R"(\d{10})">;

    // Birth date: DD-MM-YYYY
    constexpr auto isBirthDateValid = ctre::match<R"(\d{2}-\d{2}-\d{4})">;


    /*######################################
    ################ OTHER #################
    ######################################*/


    // Use a concept to constrain the type of the first param to unsigned integral only
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
            return static_cast<ReturnType>(obj);
            throw InvalidProtocolStructure{"Cannot convert " + static_cast<std::string>(obj) + " to integral type!"}; // Throwing a more specific exception
        }
    }

    std::string formatError(const std::string& functionName, const std::string& err) noexcept;
    void safePrintError(const std::string& err) noexcept;

    constexpr double calcUserScore(const uint32_t correct, const uint32_t wrong, const double avgTime) noexcept
    {
        // The formula should match the one in the DB (score row definition in user_scores)
        return (3 * correct - wrong) * (1 + 4 / avgTime);
    }

}; // namespace Helper
