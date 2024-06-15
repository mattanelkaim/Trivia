#include "LoggedUser.h"
#include "Room.h"
#include "ServerDefinitions.h"
#include <algorithm> // std::ranges::contains
#include <utility> // std::move
#include <vector>

Room::Room(RoomData data) noexcept :
    m_data(std::move(data))
{}

// NOLINTNEXTLINE(bugprone-exception-escape) - ignore std::bad_alloc
ResponseCode Room::addUser(const LoggedUser& user) noexcept
{
    if (m_users.size() >= m_data.maxPlayers)
        return ResponseCode::ROOM_IS_FULL;

    this->m_users.push_back(user);
    return ResponseCode::OK;
}

void Room::removeUser(const LoggedUser& user) noexcept
{
    std::erase(this->m_users, user);
}

const RoomData& Room::getData() const noexcept
{
    return m_data;
}

const std::vector<LoggedUser>& Room::getAllUsers() const noexcept
{
    return m_users;
}

bool Room::isUserInRoom(const LoggedUser& username) const noexcept
{
    return std::ranges::contains(this->m_users, username);
}
