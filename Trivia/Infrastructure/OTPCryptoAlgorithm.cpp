#include "OTPCryptoAlgorithm.h"
#include <cstddef> // size_t
#include <random>
#include <string>
#include <string_view>

OTPCryptoAlgorithm::OTPCryptoAlgorithm() :
    m_generator(std::random_device{}()),
    m_distribution(0, LAST_ASCII)
{}

std::string OTPCryptoAlgorithm::encrypt(const std::string_view message)
{
    std::string encrypted(message);
    m_pad = std::string(message.size(), '\0'); // Initialize a new pad

    for (size_t i = 0; i < message.size(); ++i)
    {
        m_pad[i] = static_cast<char>(m_distribution(m_generator));
        encrypted[i] ^= m_pad[i]; // XOR
    }

    return encrypted;
}

std::string OTPCryptoAlgorithm::decrypt(const std::string_view cypher) noexcept
{
    std::string decrypted(cypher);

    for (size_t i = 0; i < cypher.size(); ++i)
        decrypted[i] ^= m_pad[i]; // XOR (back)

    return decrypted;
}

const std::string& OTPCryptoAlgorithm::getPad() const noexcept
{
    return this->m_pad;
}

void OTPCryptoAlgorithm::setPad(const std::string& pad) noexcept
{
    this->m_pad = pad;
}
