#pragma once

#include "LoggedUser.h"
#include "ServerDefinitions.h"
#include <cstdint>
#include <vector>

class Room final
{
public:
    /*######################################
    ############# CONSTRUCTORS #############
    ######################################*/

    Room() = delete;
    explicit Room(RoomData data) noexcept;

    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    ResponseCode addUser(const LoggedUser& user) noexcept;
    void removeUser(const LoggedUser& user) noexcept;
    const RoomData& getData() const noexcept;
    const std::vector<LoggedUser>& getAllUsers() const noexcept;
    bool isUserInRoom(const LoggedUser& username) const noexcept;
    uint32_t getRoomId() const noexcept;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    RoomData m_data;
    std::vector<LoggedUser> m_users;
};
