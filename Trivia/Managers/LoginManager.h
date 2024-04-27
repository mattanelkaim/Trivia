#pragma once

#include "../Infrastructure/IDatabase.h"
#include "LoggedUser.h"
#include <vector>

class LoginManager
{
private:
    IDatabase* m_database;
    std::vector<LoggedUser> m_loggedUsers;

public:
    void signup(const std::string&, const std::string&, const std::string&);
    void login(const std::string&, const std::string&);
    void logout(const std::string&);
};
