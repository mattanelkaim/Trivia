#include "LoggedUser.h"

LoggedUser::LoggedUser(const std::string& username) noexcept
    : m_username(username) {}

std::string LoggedUser::getUsername() const noexcept
{
    return this->m_username;
}

bool LoggedUser::operator==(const std::string_view& otherUsername) const noexcept
{
    return this->m_username == otherUsername;
}
