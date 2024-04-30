#pragma once

#include "../Infrastructure/IDatabase.h"
#include "LoggedUser.h"
#include <string>
#include <vector>

class LoginManager
{
public:
	LoginManager();

	void signup(const std::string& username, const std::string& password, const std::string& email);
	void login(const std::string& username, const std::string& password);
	void logout(const std::string_view& username) noexcept;

private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;

	bool isUserLoggedIn(const std::string_view& username) const noexcept;
};
