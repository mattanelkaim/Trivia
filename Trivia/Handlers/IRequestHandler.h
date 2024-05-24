#pragma once

struct RequestResult; // Double-circular-jerk-dependency-linkage mega-shit
struct RequestInfo; // Double-circular-jerk-dependency-linkage mega-shit


class IRequestHandler
{
public:
    virtual ~IRequestHandler() = default;
    virtual bool isRequestRelevant(const RequestInfo& info) const noexcept = 0;
    virtual RequestResult handleRequest(const RequestInfo& info) = 0;
};
