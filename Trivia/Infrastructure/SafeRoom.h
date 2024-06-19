#pragma once

#include "Room.h"
#include "ServerDefinitions.h"
#include <atomic>
#include <cstdint>

// This is in a different file and not in serverDefinitions because of weird circular dependency
class safe_room
{
public:
    explicit safe_room(const RoomData& roomData);

    Room room;

    /*######################################
    #### AVOID SHITTY COMPILER WARNINGS ####
    ######################################*/
    safe_room operator=(const safe_room& other) = delete;
    safe_room(const safe_room& other) = default;

private:
    // friendship is not inherited for some reason
    friend class RoomAdminRequestHandler;
    friend class RoomMemberRequestHandler;
    friend class MenuRequestHandler;

    std::atomic<bool> doesRoomExist;
    std::atomic<uint16_t> numThreadsInRoom;
};
