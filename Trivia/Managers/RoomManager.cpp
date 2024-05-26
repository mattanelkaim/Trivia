#include "LoggedUser.h"
#include "Room.h"
#include "RoomManager.h"
#include "ServerDefinitions.h"
#include <cstdint>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>


void RoomManager::createRoom(const LoggedUser& user, const RoomData& data)
{
    const auto [addedRoom, isEmplaced] = this->m_rooms.try_emplace(data.id, data); // Returns pair of iterator and bool isSuccessful

    if (!isEmplaced) // Not emplaced successfully
        throw std::runtime_error("Room with id " + std::to_string(data.id) + " already exists");

    addedRoom->second.addUser(user); // Add to room the room creator
    roomIdCounter++;
}

void RoomManager::deleteRoom(const uint32_t roomId) noexcept
{
    this->m_rooms.erase(roomId);
}

uint32_t RoomManager::getRoomState(const uint32_t roomId) const
{
    return this->m_rooms.at(roomId).getData().status;
}

std::vector<RoomData> RoomManager::getRooms() const
{
    std::vector<RoomData> rooms;
    rooms.reserve(this->m_rooms.size());

    for (const auto& [_, room] : this->m_rooms)
        rooms.emplace_back(room.getData());

    return rooms;
}

Room& RoomManager::getRoom(const uint32_t roomId)
{
    return this->m_rooms.at(roomId);
}

inline uint32_t RoomManager::getNextRoomId() noexcept
{
    return roomIdCounter + 1;
}

// Singleton
std::shared_ptr<RoomManager> RoomManager::getInstance()
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    if (m_RoomManager == nullptr) [[unlikely]]
    {
        m_RoomManager = std::shared_ptr<RoomManager>(new RoomManager());
    }
    return m_RoomManager;
}
