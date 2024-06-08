/*
Things to do before shipping
- Build with compiler optimizations (O2/O3)
- Turn off ASan
*/

#include "Server.h"
#include "ServerDefinitions.h"
#include "WSAInitializer.h"
#include <exception>
#include <iostream>

#include "RoomManager.h"
static void createDummyRooms()
{
    RoomManager::getInstance()->createRoom("admin", {
        .name = "Room1",
        .id = RoomManager::getNextRoomId(),
        .maxPlayers = 6,
        .numOfQuestionsInGame = 12,
        .timePerQuestion = 10,
        .status = RoomStatus::OPEN
    });

    RoomManager::getInstance()->createRoom("adm2", {
        .name = "Room2",
        .id = RoomManager::getNextRoomId(),
        .maxPlayers = 10,
        .numOfQuestionsInGame = 7,
        .timePerQuestion = 30,
        .status = RoomStatus::CLOSED
    });

    RoomManager::getInstance()->createRoom("mattan", {
        .name = "Room3",
        .id = RoomManager::getNextRoomId(),
        .maxPlayers = 5,
        .numOfQuestionsInGame = 10,
        .timePerQuestion = 20,
        .status = RoomStatus::OPEN
    });
}

int main()
{
    try
    {
        createDummyRooms(); // TODO remove

        const WSAInitializer wsaInit;
        Server::getInstance()->run();
    }
    catch (const std::exception& e)
    {
        std::cerr << ANSI_RED << e.what() << ANSI_NORMAL << '\n';
    }

    return 0;
}
