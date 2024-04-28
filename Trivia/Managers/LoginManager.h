#pragma once

#include "../Infrastructure/IDatabase.h"
#include "LoggedUser.h"
#include <vector>

class LoginManager
{
private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
	bool isUserLoggedIn(const std::string_view& username) const noexcept;

public:
	void signup(const std::string& username, const std::string& password, const std::string& email);
	void login(const std::string& username, const std::string& password);
	void logout(const std::string_view& username) noexcept;
};
