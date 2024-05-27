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
    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    /**
    * @brief Creates a new room and adds the user to it.
    * 
    * @param user The user who is creating the room.
    * @param data The metadata of the room.
    * @throws ServerException If the room already exists.
    */
    void createRoom(const LoggedUser& user, const RoomData& data);

    void deleteRoom(uint32_t roomId) noexcept;

    /**
    * @brief Returns the state of a room.
    * 
    * @param roomId The ID of the room.
    * @return The state of the room.
    * @throws ServerException If the room does not exist.
    */
    uint32_t getRoomState(uint32_t roomId) const;

    std::vector<RoomData> getRooms() const noexcept;

    /**
    * @brief Returns a reference to a room.
    * 
    * @param roomId The ID of the room.
    * @return A reference to the room.
    * @throws ServerException If the room does not exist.
    */
    Room& getRoom(uint32_t roomId);

    static uint32_t getNextRoomId() noexcept;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    RoomManager(const RoomManager& other) = delete;
    void operator=(const RoomManager& other) = delete;
    static std::shared_ptr<RoomManager> getInstance();
    ~RoomManager() = default;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    std::unordered_map<uint32_t, Room> m_rooms;
    inline static uint32_t roomIdCounter = 0;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    RoomManager() noexcept = default;
    inline static std::shared_ptr<RoomManager> m_RoomManager = nullptr;
    inline static std::mutex m_mutex;
};
