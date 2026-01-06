#include "SafeRoom.h"
#include "ServerDefinitions.h"

safe_room::safe_room(const RoomData& roomData) :
	room(roomData),
	doesRoomExist(true),
	numThreadsInRoom(0)
{}
