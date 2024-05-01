#pragma once

#include "../ServerDefinitions.h"
#include "LoggedUser.h"
#include "Room.h"
#include <unordered_map>
#include <vector>

class RoomManager
{
public:
	void createRoom(const LoggedUser& user, const RoomData& data);
	void deleteRoom(const uint32_t& roomId) noexcept;
	uint32_t getRoomState(const uint32_t& roomId) const;
	std::vector<RoomData> getRooms() const;
	Room& getRoom(const uint32_t& roomId);

private:
	std::unordered_map<uint32_t, Room> m_rooms;
};
