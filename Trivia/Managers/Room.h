#pragma once

#include "../ServerDefinitions.h"
#include "LoggedUser.h"
#include <string>
#include <vector>

class Room
{
public:
	explicit Room(const RoomData& data);
	Room() = delete;

	void addUser(const LoggedUser& user);
	void removeUser(const LoggedUser& user);
	std::vector<std::string> getAllUsers() const;
	const RoomData& getData() const noexcept;

private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
};
