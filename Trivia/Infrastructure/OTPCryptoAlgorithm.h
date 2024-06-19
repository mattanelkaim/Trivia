#pragma once

#include "ICryptoAlgorithm.h"
#include <cstdint>
#include <random>
#include <string>
#include <string_view>


class OTPCryptoAlgorithm : public ICryptoAlgorithm
{
public:
    OTPCryptoAlgorithm();

    std::string encrypt(std::string_view message) override;
    std::string decrypt(std::string_view cypher) noexcept override;

    const std::string& getPad() const noexcept;
    void setPad(const std::string& pad) noexcept;

private:
    std::default_random_engine m_generator;
    std::uniform_int_distribution<int16_t> m_distribution;
    std::string m_pad;

    static constexpr int16_t LAST_ASCII = 127;
};
