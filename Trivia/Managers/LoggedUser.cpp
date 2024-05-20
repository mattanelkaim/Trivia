#include "LoggedUser.h"
#include <string>
#include <string_view>


LoggedUser::LoggedUser(const std::string& username) :
    m_username(username)
{}

const std::string& LoggedUser::getUsername() const noexcept
{
    return this->m_username;
}

bool LoggedUser::operator==(const std::string_view otherUsername) const noexcept
{
    return this->m_username == otherUsername;
}
