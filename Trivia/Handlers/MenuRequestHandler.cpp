#include "JsonResponseSerializer.h"
#include "MenuRequestHandler.h"
#include "ServerDefenitions.h"


bool MenuRequestHandler::isRequestRelevant(const RequestInfo& info)
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
