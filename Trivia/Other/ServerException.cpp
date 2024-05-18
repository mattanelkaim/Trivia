#include "ServerException.h"
#include <string>

ServerException::ServerException(const std::string& err) :
    m_err(err)
{}

const char* ServerException::what() const noexcept
{
    return this->m_err.c_str();
}
