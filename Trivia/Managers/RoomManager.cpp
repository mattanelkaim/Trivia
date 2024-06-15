#include "LoggedUser.h"
#include "NotFoundException.h"
#include "Room.h"
#include "RoomManager.h"
#include "ServerDefinitions.h"
#include "ServerException.h"
#include <algorithm> // std::ranges::any_of
#include <cstdint>
#include <stdexcept> // std::out_of_range
#include <string>
#include <vector>

#pragma warning(push) // To pop at the end of the file
#pragma warning(disable: 26492) // Warns about using const_cast

using std::to_string;


void RoomManager::createRoom(const LoggedUser& user, const RoomData& data)
{
    const auto [addedRoom, isEmplaced] = this->m_rooms.try_emplace(data.id, data); // Returns pair of iterator and bool isSuccessful

    if (!isEmplaced) // Not emplaced successfully
        throw ServerException("Room with id " + to_string(data.id) + " already exists");

    addedRoom->second.addUser(user); // Add to room the room creator
}

void RoomManager::deleteRoom(const uint32_t roomId) noexcept
{
    this->m_rooms.erase(roomId);
}

RoomStatus RoomManager::getRoomState(const uint32_t roomId) const
{
    //NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast) - ignore const_cast
    const Room& room = const_cast<RoomManager*>(this)->getRoom(roomId); // Must use const_cast because getRoom is not const, oof
    return room.getData().status;
}

// NOLINTNEXTLINE(bugprone-exception-escape) - ignore reserve and std::bad_alloc
std::vector<RoomData> RoomManager::getRooms() const noexcept
{
    std::vector<RoomData> rooms;
    rooms.reserve(this->m_rooms.size());

    for (const auto& [_, room] : this->m_rooms)
        rooms.emplace_back(room.getData());

    return rooms;
}

Room& RoomManager::getRoom(const uint32_t roomId)
{
    // Simply rethrows the exception thrown by at()
    try
    {
        return this->m_rooms.at(roomId);
    }
    catch (const std::out_of_range&)
    {
        throw NotFoundException("Room with ID " + to_string(roomId));
    }
}

bool RoomManager::isUserInAnyRoom(const LoggedUser& user) const noexcept
{
    // Use a lambda on each room to check if user is in it
    return std::ranges::any_of(this->m_rooms, [&](const auto& roomPair) noexcept {
        return roomPair.second.isUserInRoom(user); // Check if user is in room
    });
}

uint32_t RoomManager::getNextRoomId() noexcept
{
    return ++roomIdCounter; // Increment then return
}

// Singleton
RoomManager& RoomManager::getInstance() noexcept
{
    static RoomManager instance; // This is thread-safe in C++11 and later
    return instance;
}

#pragma warning(pop) // Popping C26492
