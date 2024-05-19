#pragma once

#include "LoggedUser.h"
#include "ServerDefinitions.h"
#include <string>
#include <vector>

class Room final
{
public:
    /*######################################
    ############# CONSTRUCTORS #############
    ######################################*/

    Room() = delete;
    explicit Room(const RoomData& data) noexcept;

    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    void addUser(const LoggedUser& user);
    void removeUser(const LoggedUser& user) noexcept;
    std::vector<std::string> getAllUsers() const;
    const RoomData& getData() const noexcept;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    RoomData m_metadata;
    std::vector<LoggedUser> m_users;
};
