#include "InvalidProtocolStructure.h"
#include "ServerException.h"
#include <string>


InvalidProtocolStructure::InvalidProtocolStructure() noexcept :
    ServerException("Invalid protocol structure!")
{}

InvalidProtocolStructure::InvalidProtocolStructure(const std::string& query) :
    ServerException("Invalid protocol structure: " + query)
{}
