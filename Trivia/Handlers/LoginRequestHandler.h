#pragma once

#include "IRequestHandler.h"

class LoginRequestHandler : public IRequestHandler
{
    bool isRequestRelevant(const RequestInfo& info) override;
    RequestResult handleRequest(const RequestInfo& info) override;
};
