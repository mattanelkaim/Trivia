#include "LoggedUser.h"
#include "Room.h"
#include "ServerDefinitions.h"
#include <utility> // std::move
#include <vector>

Room::Room(RoomData data) noexcept :
    m_metadata(std::move(data))
{}

// NOLINTNEXTLINE(bugprone-exception-escape) - ignore std::bad_alloc
void Room::addUser(const LoggedUser& user) noexcept
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
