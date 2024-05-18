#include "InvalidProtocolStructure.h"
#include <string>


InvalidProtocolStructure::InvalidProtocolStructure() :
    ServerException("Invalid protocol structure!")
{}

InvalidProtocolStructure::InvalidProtocolStructure(const std::string& invalidPart) :
    ServerException("Invalid protocol structure: " + invalidPart)
{}

const char* InvalidProtocolStructure::what() const noexcept
{
    return this->m_err.c_str();
}
