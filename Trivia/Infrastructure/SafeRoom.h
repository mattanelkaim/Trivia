#pragma once

#include "Room.h"
#include "ServerDefinitions.h"
#include <atomic>
#include <cstdint>

// This is a separate file and not in serverDefinitions because of a weird circular dependency
class safe_room final
{
public:
    explicit safe_room(const RoomData& roomData);

    Room room;

    /*######################################
    #### AVOID SHITTY COMPILER WARNINGS ####
    ######################################*/
    safe_room operator=(const safe_room& other) = delete;
    safe_room(const safe_room& other) = delete;

private:
    /*######################################
    ################ FRIENDS ###############
    ######################################*/

    // friendship is not inherited for some reason
    friend class RoomAdminRequestHandler;
    friend class RoomMemberRequestHandler;
    friend class MenuRequestHandler;

    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    std::atomic<bool> doesRoomExist;
    std::atomic<uint16_t> numThreadsInRoom;
};
