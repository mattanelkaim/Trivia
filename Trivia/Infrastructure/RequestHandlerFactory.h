#pragma once

#include "LoggedUser.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

class RequestHandlerFactory final
{
public:
    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    static MenuRequestHandler* createMenuRequestHandler(const LoggedUser& user);
    static RoomAdminRequestHandler* createRoomAdminRequestHandler(const LoggedUser& user, Room room);
    static RoomMemberRequestHandler* createRoomMemberRequestHandler(const LoggedUser& user, Room room);


    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    RequestHandlerFactory(const RequestHandlerFactory& other) = delete;
    void operator=(const RequestHandlerFactory& other) = delete;
    static RequestHandlerFactory& getInstance() noexcept;
    ~RequestHandlerFactory() noexcept = default;

private:
    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    RequestHandlerFactory() noexcept = default;
};
