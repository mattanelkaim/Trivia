#include "LoginManager.h"


LoginManager::LoginManager()
	: m_database(nullptr) {}

void LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
	this->m_database->addNewUser(username, password, email);
}

void LoginManager::login(const std::string& username, const std::string& password)
{
	// Log in if username exists with correct password, and not already logged in
	if (m_database->doesUserExist(username) && m_database->doesPasswordMatch(username, password) && !isUserLoggedIn(username))
		this->m_loggedUsers.emplace_back(username); // Calls LoggedUser constructor
}

void LoginManager::logout(const std::string_view& username) noexcept
{
	std::erase(this->m_loggedUsers, username);
}


bool LoginManager::isUserLoggedIn(const std::string_view& username) const noexcept
{
	return std::find(m_loggedUsers.cbegin(), m_loggedUsers.cend(), username) != m_loggedUsers.cend();
}
