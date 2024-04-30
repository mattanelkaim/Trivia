#include "../ServerDefenitions.h"
#include "MenuRequestHandler.h"


bool MenuRequestHandler::isRequestRelevant(const RequestInfo& info)
{
    return true;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& info)
{
    return RequestResult{};
}
