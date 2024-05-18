#pragma once

#include "IDatabase.h"
#include "LoggedUser.h"
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

class LoginManager
{
public:
    bool signup(const std::string& username, const std::string& password, const std::string& email);
    bool login(const std::string& username, const std::string& password);
    void logout(std::string_view username) noexcept;

    // Singleton
    LoginManager() = delete;
    LoginManager(LoginManager& other) = delete;
    void operator=(const LoginManager& other) = delete;
    static std::unique_ptr<LoginManager>& getInstance(IDatabase* db);
    ~LoginManager() = default;

private:
    // Members
    IDatabase* m_database;
    std::vector<LoggedUser> m_loggedUsers;

    // Private methods
    bool isUserLoggedIn(std::string_view username) const noexcept;

    // Singleton
    explicit LoginManager(IDatabase* db) noexcept;
    inline static std::unique_ptr<LoginManager> m_LoginManager = nullptr;
    inline static std::mutex m_mutex;
};
