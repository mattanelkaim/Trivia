#include "IDatabase.h"
#include "LoginManager.h"
#include <algorithm> // std::find
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector> // std::erase

LoginManager::LoginManager(IDatabase* db) noexcept :
    m_database(db)
{}

bool LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
    // if (!m_database->doesUserExist(username)) - no need for this as SQL's 'UNIQUE' keywprd already takes care of this for us
    try
    {
        this->m_database->addNewUser(username, password, email);
        return true;
    }
    catch(...) // some SQL error
    {
        return false; // User or Email already exists
    }
}

bool LoginManager::login(const std::string& username, const std::string& password)
{
    // Log in if username exists with correct password, and not already logged in
    if (m_database->doesUserExist(username) && m_database->doesPasswordMatch(username, password) && !isUserLoggedIn(username))
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
LoginManager* LoginManager::getInstance(IDatabase* db)
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    if (m_LoginManager == nullptr) [[unlikely]]
    {
        m_LoginManager = std::unique_ptr<LoginManager>(new LoginManager(db));
    }
    return m_LoginManager.get();
}
