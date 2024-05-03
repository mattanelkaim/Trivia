#pragma once

#include "IServerException.h"

class InvalidProtocolStructure : public IServerException
{
public:
	InvalidProtocolStructure();
	explicit InvalidProtocolStructure(const std::string& invalidPart);
	const char* what() const override;
};

