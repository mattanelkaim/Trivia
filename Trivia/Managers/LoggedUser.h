#pragma once

#include <string>
#include <string_view>

class LoggedUser final
{
public:
    LoggedUser() = delete;
    explicit LoggedUser(const std::string& username);
    const std::string& getUsername() const noexcept;

    bool operator==(const LoggedUser& other) const noexcept;
    bool operator==(std::string_view otherUsername) const noexcept;

private:
    std::string m_username;
};
