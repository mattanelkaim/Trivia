#pragma once

#include "LoggedUser.h"
#include "MenuRequestHandler.h"

class RequestHandlerFactory final
{
public:
    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    MenuRequestHandler* createMenuRequestHandler(const LoggedUser& user);

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    RequestHandlerFactory(const RequestHandlerFactory& other) = delete;
    void operator=(const RequestHandlerFactory& other) = delete;
    static RequestHandlerFactory& getInstance() noexcept;
    ~RequestHandlerFactory() noexcept = default;

private:
    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    RequestHandlerFactory() noexcept = default;
};
