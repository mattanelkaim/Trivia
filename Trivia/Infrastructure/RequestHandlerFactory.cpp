#include "LoggedUser.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"


MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& user)
{
    return new MenuRequestHandler(user);
}

// Singleton
RequestHandlerFactory& RequestHandlerFactory::getInstance() noexcept
{
    static RequestHandlerFactory instance; // This is thread-safe in C++11 and later
    return instance;
}
