#pragma once

#include <exception>
#include <string>

class ServerException : public std::exception
{
public:
    ServerException() = default;
    explicit ServerException(const std::string& err) : m_err(err) {}
    const char* what() const noexcept override { return this->m_err.c_str(); }

protected:
    std::string m_err;
};
