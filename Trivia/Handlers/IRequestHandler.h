#pragma once

struct RequestResult; // Double-circular-jerk-dependency-linkage mega-shit
struct RequestInfo; // Double-circular-jerk-dependency-linkage mega-shit


class IRequestHandler
{
public:
	virtual ~IRequestHandler() = default;
	
private:
	virtual bool isRequestRelevant(const RequestInfo& info) = 0;
	virtual RequestResult handleRequest(const RequestInfo& info) = 0;
};
