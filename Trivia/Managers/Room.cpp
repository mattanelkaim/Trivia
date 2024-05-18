#include "LoggedUser.h"
#include "Room.h"
#include "ServerDefinitions.h"
#include <string>
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

std::vector<std::string> Room::getAllUsers() const
{
    std::vector<std::string> users;
    users.reserve(this->m_users.size());

    for (const LoggedUser& user : this->m_users)
        users.push_back(user.getUsername());

    return users;
}

const RoomData& Room::getData() const noexcept
{
    return this->m_metadata;
}
