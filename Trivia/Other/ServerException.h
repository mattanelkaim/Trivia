#pragma once

#include <exception>
#include <string>

class ServerException : public std::exception
{
public:
    ServerException() noexcept = default;
    explicit ServerException(std::string err) noexcept;
    const char* what() const noexcept override;

protected:
    std::string m_err;
};
