#pragma once

#include <exception>
#include <string>

class ServerException : public std::exception
{
public:
    ServerException() = default;
    explicit ServerException(const std::string& err);
    const char* what() const noexcept override;

protected:
    std::string m_err;
};
