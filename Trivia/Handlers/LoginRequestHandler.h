#pragma once

#include "IRequestHandler.h"

class LoginRequestHandler : public IRequestHandler
{
	virtual bool isRequestRelevant(const RequestInfo& info) override;
	virtual RequestResult handleRequest(const RequestInfo& info) override;
};
