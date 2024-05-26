#pragma once

#include "LoggedUser.h"
#include "Room.h"
#include "ServerDefinitions.h"
#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

class RoomManager final
{
public:
    void createRoom(const LoggedUser& user, const RoomData& data);
    void deleteRoom(uint32_t roomId) noexcept;
    
    uint32_t getRoomState(uint32_t roomId) const;
    std::vector<RoomData> getRooms() const;
    Room& getRoom(uint32_t roomId);
    inline static uint32_t getNextRoomId() { return roomIdCounter + 1; };

    // Singleton
    RoomManager(RoomManager& other) = delete;
    void operator=(const RoomManager& other) = delete;
    static std::shared_ptr<RoomManager> getInstance();
    ~RoomManager() = default;

private:
    std::unordered_map<uint32_t, Room> m_rooms;
    inline static uint32_t roomIdCounter = 0;
    // Singleton
    RoomManager() noexcept = default;
    inline static std::shared_ptr<RoomManager> m_RoomManager = nullptr;
    inline static std::mutex m_mutex;
};
