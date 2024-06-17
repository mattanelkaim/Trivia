#pragma once

#include "ServerException.h"
#include <string>

class NotFoundException final : public ServerException
{
public:
    NotFoundException() noexcept;
    explicit NotFoundException(const std::string& item) noexcept;
};
