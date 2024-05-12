#include "LoggedUser.h"

LoggedUser::LoggedUser(const std::string& username) :
    m_username(username)
{}

std::string LoggedUser::getUsername() const noexcept
{
    return this->m_username;
}

bool LoggedUser::operator==(const std::string_view otherUsername) const noexcept
{
    return this->m_username == otherUsername;
}

bool LoggedUser::operator==(const LoggedUser& other) const noexcept
{
    return this->m_username == other.m_username;
}
