#pragma once

#include "Room.h"
#include "ServerDefinitions.h"
#include <atomic>

// This is in a differnt file and not in serverDefenitions because of weird circular dependency
class safe_room
{
public:
    safe_room(const RoomData& roomData);

    Room room;

private:
    // friendship is not inherited for some reason
    friend class RoomAdminRequestHandler;
    friend class RoomMemberRequestHandler;
    friend class MenuRequestHandler;

    std::atomic<bool> doesRoomExist;
    std::atomic<uint16_t> numThreadsInRoom;
};