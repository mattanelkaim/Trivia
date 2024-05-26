#include "LoggedUser.h"
#include "Room.h"
#include "ServerDefinitions.h"
#include <vector>


Room::Room(const RoomData& data) noexcept :
    m_metadata(data)
{}

void Room::addUser(const LoggedUser& user)
{
    this->m_users.push_back(user);
}

void Room::removeUser(const LoggedUser& user) noexcept
{
    std::erase(this->m_users, user);
}

const RoomData& Room::getData() const noexcept
{
    return m_metadata;
}

const std::vector<LoggedUser>& Room::getAllUsers() const noexcept
{
    return m_users;
}
