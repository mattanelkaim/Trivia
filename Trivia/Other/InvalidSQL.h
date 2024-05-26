#pragma once

#include "ServerException.h"
#include <string>

class InvalidSQL : public ServerException
{
public:
    InvalidSQL() = delete;
    explicit InvalidSQL(const std::string& query);
};
