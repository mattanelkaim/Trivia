#include "InvalidProtocolStructure.h"
#include <string>


InvalidProtocolStructure::InvalidProtocolStructure() :
    ServerException("Invalid protocol structure!")
{}

InvalidProtocolStructure::InvalidProtocolStructure(const std::string& invalidPart) :
    ServerException("Invalid protocol structure: " + invalidPart)
{}
