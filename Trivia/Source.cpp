/*
Things to do before shipping
- Build with compiler optimizations (O2/O3)
- Turn off ASan
*/

#include "Helper.h"
#include "Server.h"
#include "ServerDefinitions.h"
#include "SqliteDatabase.h"
#include "WSAInitializer.h"
#include <exception>
#include <iostream>

#include "RoomManager.h"
static void createDummyRooms() noexcept
{
    RoomManager::getInstance().createRoom("MMMMMMMMMMMMMMMM", {
        .name = "Room1",
        .id = RoomManager::getNextRoomId(),
        .maxPlayers = 6,
        .numOfQuestionsInGame = 12,
        .timePerQuestion = 10,
        .status = RoomStatus::OPEN
    });

    RoomManager::getInstance().createRoom("adm2", {
        .name = "Room2",
        .id = RoomManager::getNextRoomId(),
        .maxPlayers = 10,
        .numOfQuestionsInGame = 7,
        .timePerQuestion = 30,
        .status = RoomStatus::OPEN
    });

    RoomManager::getInstance().createRoom("mattanigger", {
        .name = "Room3",
        .id = RoomManager::getNextRoomId(),
        .maxPlayers = 200,
        .numOfQuestionsInGame = 10,
        .timePerQuestion = 20,
        .status = RoomStatus::OPEN
    });
}

int main()
{
    try
    {
        createDummyRooms(); // TODO(mattan) remove

        const WSAInitializer wsaInit;
        SqliteDatabase::getInstance().openDB();
        Server::getInstance().run();
    }
    catch (const std::exception& e)
    {
        Helper::safePrintError(Helper::formatError(__FUNCTION__, e.what()));
    }

    return 0;
}
