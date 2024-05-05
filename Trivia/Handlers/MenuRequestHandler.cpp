#include "JsonResponseSerializer.h"
#include "MenuRequestHandler.h"
#include "ServerDefinitions.h"


bool MenuRequestHandler::isRequestRelevant(const RequestInfo& info) const
{
    return info.id != LOGIN && info.id != SIGNUP;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& info)
{
    RequestResult result;
    result.response = JsonResponseSerializer::serializeErrorResponse(ErrorResponse{"Not supported yet"});
    result.newHandler = new MenuRequestHandler;
    return result;
}
