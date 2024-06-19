#pragma once

#include <string>

class ICryptoAlgorithm
{
public:
    virtual std::string encrypt(const std::string& key) = 0;
    virtual std::string decrypt(const std::string& key) = 0;
};
