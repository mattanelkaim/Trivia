#pragma once

#include <string>

class LoggedUser
{
private:
    std::string m_username;

public:
    LoggedUser() = delete;
    explicit LoggedUser(const std::string& username) noexcept;
    std::string getUsername() const noexcept;
};
