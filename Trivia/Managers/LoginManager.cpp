#include "InvalidSQL.h"
#include "LoginManager.h"
#include "SqliteDatabase.h"
#include <algorithm> // std::find
#include <string>
#include <string_view>
#include <vector> // std::erase


bool LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
    // if (!m_database->doesUserExist(username)) - no need for this as SQL's 'UNIQUE' keyword already takes care of this for us
    try
    {
        SqliteDatabase::getInstance().addNewUser(username, password, email);
        return login(username, password);
    }
    catch(const InvalidSQL&) // some SQL error
    {
        return false; // User or Email already exists
    }
}

bool LoginManager::login(const std::string& username, const std::string& password)
{
    // Log in if username exists with correct password, and not already logged in
    if (SqliteDatabase::getInstance().doesUserExist(username) && SqliteDatabase::getInstance().doesPasswordMatch(username, password) && !isUserLoggedIn(username))
    {
        this->m_loggedUsers.emplace_back(username); // Calls LoggedUser constructor
        return true;
    }
    return false; // Login failed
}

void LoginManager::logout(const std::string_view username) noexcept
{
    std::erase(this->m_loggedUsers, username);
}

bool LoginManager::isUserLoggedIn(const std::string_view username) const noexcept
{
    return std::find(m_loggedUsers.cbegin(), m_loggedUsers.cend(), username) != m_loggedUsers.cend();
}

// Singleton
LoginManager& LoginManager::getInstance() noexcept
{
    static LoginManager instance; // This is thread-safe in C++11 and later
    return instance;
}
