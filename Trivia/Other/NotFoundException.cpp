#include "NotFoundException.h"

NotFoundException::NotFoundException() noexcept :
    ServerException("Not found!")
{}

NotFoundException::NotFoundException(const std::string& item) noexcept :
    ServerException("Not found: " + item)
{}
