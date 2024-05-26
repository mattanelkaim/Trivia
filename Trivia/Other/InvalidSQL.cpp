#include "InvalidSQL.h"
#include "ServerException.h"
#include <string>

InvalidSQL::InvalidSQL(const std::string& query) :
    ServerException(query)
{}
