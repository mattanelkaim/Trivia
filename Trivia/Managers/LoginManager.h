#pragma once

#include "IDatabase.h"
#include "LoggedUser.h"
#include <mutex>
#include <string>
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
	static LoginManager* getInstance(IDatabase* db);

private:
	// Members
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;

	// Private methods
	bool isUserLoggedIn(std::string_view username) const noexcept;

	// Singleton
	explicit LoginManager(IDatabase* db);
	~LoginManager() = default;
	inline static LoginManager* m_LoginManager = nullptr;
	static std::mutex m_mutex;
};
