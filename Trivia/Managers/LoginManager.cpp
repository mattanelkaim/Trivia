#include "LoggedUser.h"
#include "LoginManager.h"


bool LoginManager::isUserLoggedIn(const std::string_view& username) const noexcept
{
	auto isIdenticalUsername = [username](const LoggedUser& user) noexcept { return user.getUsername() == username; };
	return std::find_if(m_loggedUsers.cbegin(), m_loggedUsers.cend(), isIdenticalUsername) != m_loggedUsers.cend();
}

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
	auto isIdenticalUsername = [username](const LoggedUser& user) noexcept { return user.getUsername() == username; };
	std::erase_if(this->m_loggedUsers, isIdenticalUsername);
}
