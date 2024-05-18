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

    // Singleton
    RoomManager(RoomManager& other) = delete;
    void operator=(const RoomManager& other) = delete;
    static std::unique_ptr<RoomManager>& getInstance();
    ~RoomManager() = default;

private:
    std::unordered_map<uint32_t, Room> m_rooms;

    // Singleton
    RoomManager() noexcept = default;
    inline static std::unique_ptr<RoomManager> m_RoomManager = nullptr;
    inline static std::mutex m_mutex;
};
