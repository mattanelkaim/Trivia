#pragma once

#include "ServerException.h"
#include <string>

class InvalidProtocolStructure final : public ServerException
{
public:
    InvalidProtocolStructure();
    explicit InvalidProtocolStructure(const std::string& query);
};
