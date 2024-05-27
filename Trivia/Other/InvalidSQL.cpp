#include "InvalidSQL.h"
#include "ServerException.h"
#include <string>

InvalidSQL::InvalidSQL(const std::string& query) noexcept :
    ServerException(query)
{}
