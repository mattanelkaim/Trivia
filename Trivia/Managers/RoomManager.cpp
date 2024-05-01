#include "RoomManager.h"
#include <stdexcept>
#include <string>


void RoomManager::createRoom(const LoggedUser& user, const RoomData& data)
{
	if (!(this->m_rooms.try_emplace(data.id, data).second))
		throw std::runtime_error("Room with id " + std::to_string(data.id) + " already exists");
}

void RoomManager::deleteRoom(const uint32_t& roomId) noexcept
{
	this->m_rooms.erase(roomId);
}

uint32_t RoomManager::getRoomState(const uint32_t& roomId) const
{
	return this->m_rooms.at(roomId).getData().status;
}

std::vector<RoomData> RoomManager::getRooms() const
{
	std::vector<RoomData> users;
	for (const auto& [_, room] : this->m_rooms)
		users.push_back(room.getData());
	return users;
}

Room& RoomManager::getRoom(const uint32_t& roomId)
{
	return (this->m_rooms.at(roomId));
}
