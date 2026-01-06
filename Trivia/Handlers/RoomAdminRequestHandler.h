#pragma once

#include "IRoomRequestHandler.h"
#include "LoggedUser.h"
#include "SafeRoom.h"
#include "ServerDefinitions.h"

class RoomAdminRequestHandler final : public IRoomRequestHandler
{
public:
    RoomAdminRequestHandler(LoggedUser user, safe_room& room);
    ~RoomAdminRequestHandler() noexcept override;
    
    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    bool isRequestRelevant(const RequestInfo& requestInfo) const noexcept override;
    RequestResult handleRequest(const RequestInfo& requestInfo) noexcept override;

    /*######################################
    #### AVOID SHITTY COMPILER WARNINGS ####
    ######################################*/
    RoomAdminRequestHandler operator=(const RoomAdminRequestHandler& other) = delete;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

private:
	mutable bool m_hasExitedSafely;	
	RequestResult startGameRequest() const noexcept;
	RequestResult closeRoomRequest() noexcept;
};
