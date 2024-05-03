#include "RoomManager.h"
#include <stdexcept>
#include <string>


void RoomManager::createRoom(const LoggedUser& user, const RoomData& data)
{
	const auto emplaced = this->m_rooms.try_emplace(data.id, data); // Returns pair of iterator and bool isSuccessful
	if (!emplaced.second) // Not emplaced successfully
		throw std::runtime_error("Room with id " + std::to_string(data.id) + " already exists");
	emplaced.first->second.addUser(user); // Add to room the room creator
}

void RoomManager::deleteRoom(uint32_t roomId) noexcept
{
	this->m_rooms.erase(roomId);
}

uint32_t RoomManager::getRoomState(uint32_t roomId) const
{
	return this->m_rooms.at(roomId).getData().status;
}

std::vector<RoomData> RoomManager::getRooms() const
{
	std::vector<RoomData> rooms;
	for (const auto& [_, room] : this->m_rooms)
		rooms.push_back(room.getData());
	return rooms;
}

Room& RoomManager::getRoom(uint32_t roomId)
{
	return (this->m_rooms.at(roomId));
}
