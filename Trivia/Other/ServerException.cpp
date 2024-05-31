#include "ServerException.h"
#include <string>
#include <utility> // std::move

ServerException::ServerException(std::string err) noexcept :
    m_err(std::move(err))
{}

const char* ServerException::what() const noexcept
{
    return this->m_err.c_str();
}
