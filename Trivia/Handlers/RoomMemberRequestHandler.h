#pragma once

#include "IRoomRequestHandler.h"
#include "LoggedUser.h"
#include "SafeRoom.h"
#include "ServerDefinitions.h"

class RoomMemberRequestHandler final : public IRoomRequestHandler
{
public:
    RoomMemberRequestHandler(LoggedUser user, safe_room& room) noexcept;
    ~RoomMemberRequestHandler() noexcept override;
    
    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    bool isRequestRelevant(const RequestInfo& requestInfo) const noexcept override;
    RequestResult handleRequest(const RequestInfo& requestInfo) noexcept override;

    /*######################################
    #### AVOID SHITTY COMPILER WARNINGS ####
    ######################################*/
    RoomMemberRequestHandler operator=(const RoomMemberRequestHandler& other) = delete;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    mutable bool m_hasExitedSafely;
    RequestResult leaveRoom() noexcept;
    bool wasRoomClosed() const noexcept;
};
