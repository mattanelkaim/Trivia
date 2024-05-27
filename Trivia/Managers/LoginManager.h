#pragma once

#include "IDatabase.h"
#include "LoggedUser.h"
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

class LoginManager final
{
public:
    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    /**
    * @brief Registers a new user.
    * 
    * @param username The username of the new user.
    * @param password The password of the new user.
    * @param email The email of the new user.
    * @return true if the user was successfully registered, false if the username already exists.
    * @throws InvalidSQL
    */
    bool signup(const std::string& username, const std::string& password, const std::string& email);

    /**
    * @brief Logs in a user.
    * 
    * @param username The username of the user.
    * @param password The password of the user.
    * @return true if the login was successful, false otherwise.
    * @throws InvalidSQL
    */
    bool login(const std::string& username, const std::string& password);

    void logout(std::string_view username) noexcept;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    LoginManager() = delete;
    LoginManager(const LoginManager& other) = delete;
    void operator=(const LoginManager& other) = delete;
    static LoginManager* getInstance(IDatabase* db);
    ~LoginManager() = default;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    IDatabase* m_database;
    std::vector<LoggedUser> m_loggedUsers;

    /*######################################
    ############ PRIVATE METHODS ###########
    ######################################*/

    bool isUserLoggedIn(std::string_view username) const noexcept;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    explicit LoginManager(IDatabase* db) noexcept;
    inline static std::unique_ptr<LoginManager> m_LoginManager = nullptr;
    inline static std::mutex m_mutex;
};
