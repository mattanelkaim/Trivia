#pragma once

#include <string>

class ICryptoAlgorithm
{
public:
    virtual ~ICryptoAlgorithm() noexcept = default;

    virtual std::string encrypt(const std::string& message) = 0;
    virtual std::string decrypt(const std::string& cypher) = 0;
};
