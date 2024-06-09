#pragma once

#include "ServerDefinitions.h"

class IRequestHandler
{
public:
    virtual ~IRequestHandler() noexcept = default;
    virtual bool isRequestRelevant(const RequestInfo& info) const noexcept = 0;
    virtual RequestResult handleRequest(const RequestInfo& info) = 0;
};
