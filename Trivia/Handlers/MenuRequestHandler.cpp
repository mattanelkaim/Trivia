#include "JsonResponseSerializer.h"
#include "MenuRequestHandler.h"
#include "ServerDefinitions.h"


bool MenuRequestHandler::isRequestRelevant(const RequestInfo& info) const noexcept
{
    return true; // info.id != LOGIN && info.id != SIGNUP;
}

RequestResult MenuRequestHandler::handleRequest([[maybe_unused]] const RequestInfo& info)
{
    return RequestResult{
        .response = JsonResponseSerializer::serializeResponse(ErrorResponse{"Not supported yet"}),
        .newHandler = new MenuRequestHandler()
    };
}
