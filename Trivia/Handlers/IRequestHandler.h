#pragma once

#include "../ServerDefenitions.h"

struct RequestResult; // Double-linkage shit

class IRequestHandler
{
	virtual bool isRequestRelevant(const RequestInfo& info) = 0;
	virtual RequestResult handleRequest(const RequestInfo& info) = 0;
};
