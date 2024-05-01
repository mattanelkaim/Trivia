#include "LoginManager.h"

// Singleton
LoginManager* m_LoginManager = nullptr;
std::mutex LoginManager::m_mutex;


LoginManager* LoginManager::getInstance(IDatabase* db)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_LoginManager == nullptr)
	{
		m_LoginManager = new LoginManager(db);
	}
	return m_LoginManager;}

LoginManager::LoginManager(IDatabase* db)
	: m_database(db) {}

bool LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
	if (!m_database->doesUserExist(username))
	{
		this->m_database->addNewUser(username, password, email);
		return true;
	}
	return false; // User already exists
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

void LoginManager::logout(const std::string_view& username) noexcept
{
	std::erase(this->m_loggedUsers, username);
}

bool LoginManager::isUserLoggedIn(const std::string_view& username) const noexcept
{
	return std::find(m_loggedUsers.cbegin(), m_loggedUsers.cend(), username) != m_loggedUsers.cend();
}
