#pragma once

#include "ServerException.h"
#include <string>

class InvalidProtocolStructure final : public ServerException
{
public:
    InvalidProtocolStructure() noexcept;
    explicit InvalidProtocolStructure(const std::string& query);
};
