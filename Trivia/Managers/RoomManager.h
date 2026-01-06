#pragma once

#include "LoggedUser.h"
#include "SafeRoom.h"
#include "ServerDefinitions.h"
#include <cstdint>
#include <string_view>
#include <unordered_map>
#include <vector>

class RoomManager final
{
public:
    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    /**
    * Creates a new room and adds the user (admin) to it.
    * @param user The user who is creating the room.
    * @param data The metadata of the room.
    * @return A copy of created room.
    */
    safe_room& createRoom(const LoggedUser& user, const RoomData& data) noexcept;

    void deleteRoom(uint32_t roomId) noexcept;

    /**
    * Returns the state of a room.
    * @param roomId The ID of the room.
    * @return The state of the room.
    * @throws NotFoundException
    */
    RoomStatus getRoomState(uint32_t roomId) const;

    std::vector<RoomData> getRooms() const noexcept;

    /**
    * Returns a reference to a room.
    * @param roomId The ID of the room.
    * @return A reference to the room.
    * @throws NotFoundException
    */
    safe_room& getRoom(uint32_t roomId);

    bool doesRoomExist(std::string_view roomName) const noexcept;

    /**
     * This method increments the static room ID counter and returns the new value.
     * It is used to assign a unique ID to each new room created.
     * @return The next unique room ID.
     */
    static uint32_t getNextRoomId() noexcept;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    RoomManager(const RoomManager& other) = delete;
    void operator=(const RoomManager& other) = delete;
    static RoomManager& getInstance() noexcept;
    ~RoomManager() noexcept = default;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    std::unordered_map<uint32_t, safe_room> m_rooms;
    inline static uint32_t roomIdCounter = 0;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    RoomManager() noexcept = default;
};
