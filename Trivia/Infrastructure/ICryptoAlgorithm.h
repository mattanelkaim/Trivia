#pragma once

#include <string>
#include <string_view>

class ICryptoAlgorithm
{
public:
    virtual ~ICryptoAlgorithm() noexcept = default;

    virtual std::string encrypt(std::string_view message) = 0;
    virtual std::string decrypt(std::string_view cypher) = 0;
};
