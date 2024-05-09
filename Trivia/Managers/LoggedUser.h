#pragma once

#include <string>

class LoggedUser
{
public:
    LoggedUser() = delete;
    explicit LoggedUser(const std::string& username);
    std::string getUsername() const noexcept;

    bool operator==(const std::string_view otherUsername) const noexcept;
    bool operator==(const LoggedUser& other) const noexcept;

private:
    std::string m_username;
};
