#pragma once

#include "IRequestHandler.h"

class MenuRequestHandler final : public IRequestHandler
{
public:
    bool isRequestRelevant(const RequestInfo& info) const noexcept override;
    RequestResult handleRequest(const RequestInfo& info) override;
};
