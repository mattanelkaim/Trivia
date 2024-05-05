#include "InvalidProtocolStructure.h"

InvalidProtocolStructure::InvalidProtocolStructure() :
	IServerException("Invalid protocol structure!")
{}

InvalidProtocolStructure::InvalidProtocolStructure(const std::string& invalidPart) :
	IServerException("Invalid protocol structure: " + invalidPart)
{}

const char* InvalidProtocolStructure::what() const
{
	return this->m_err.c_str();
}
