#pragma once

#include <string>

class IDatabase
{
public:
	virtual ~IDatabase() = default;
	virtual bool doesUserExist(const std::string& username) const = 0;
	virtual bool doesPasswordMatch(const std::string&, const std::string&) const = 0;
	virtual void addNewUser(const std::string&, const std::string&, const std::string&) = 0;
};
