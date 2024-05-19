#pragma once

#include <string>
#include <string_view>

class LoggedUser final
{
public:
    /*######################################
    ############# CONSTRUCTORS #############
    ######################################*/

    LoggedUser() = delete;
    explicit LoggedUser(const std::string& username);

    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    const std::string& getUsername() const noexcept;

    /*######################################
    ######### OPERATORS OVERLOADING ########
    ######################################*/

    bool operator==(const LoggedUser& other) const noexcept;
    bool operator==(std::string_view otherUsername) const noexcept;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    std::string m_username;
};
