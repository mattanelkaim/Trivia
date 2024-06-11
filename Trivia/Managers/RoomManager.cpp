#include "LoggedUser.h"
#include "Room.h"
#include "RoomManager.h"
#include "ServerDefinitions.h"
#include "ServerException.h"
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

using std::to_string;


void RoomManager::createRoom(const LoggedUser& user, const RoomData& data)
{
    const auto [addedRoom, isEmplaced] = this->m_rooms.try_emplace(data.id, data); // Returns pair of iterator and bool isSuccessful

    if (!isEmplaced) // Not emplaced successfully
        throw ServerException("Room with id " + to_string(data.id) + " already exists");

    addedRoom->second.addUser(user); // Add to room the room creator
    roomIdCounter++;
}

void RoomManager::deleteRoom(const uint32_t roomId) noexcept
{
    this->m_rooms.erase(roomId);
}

uint32_t RoomManager::getRoomState(const uint32_t roomId) const
{
    try
    {
        return this->m_rooms.at(roomId).getData().status;
    }
    catch (const std::out_of_range&)
    {
        throw ServerException("Cannot find a room with ID " + to_string(roomId));
    }
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
    try
    {
        return this->m_rooms.at(roomId);
    }
    catch (const std::out_of_range&)
    {
        throw ServerException("Cannot find a room with ID " + to_string(roomId));
    }
}

uint32_t RoomManager::getNextRoomId() noexcept
{
    return ++roomIdCounter;
}

// Singleton
RoomManager& RoomManager::getInstance() noexcept
{
    static RoomManager instance; // This is thread-safe in C++11 and later
    return instance;
}
