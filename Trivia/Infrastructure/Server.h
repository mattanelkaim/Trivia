#pragma once

#include "Communicator.h"
#include "IDatabase.h"
#include "RequestHandlerFactory.h"

class Server final
{
public:
    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    /**
    * @brief Starts the server and handles user input.
    * 
    * This method starts a new thread that handles client requests. It then enters a loop where it reads user input
    * from the console and executes commands. The loop continues until the user enters the "exit" command.
    */
    void run();

    /*######################################
    ############### SINGLETON ##############
    ######################################*/
    
    Server(const Server& other) = delete;
    void operator=(const Server& other) = delete;
    static Server& getInstance();
    ~Server() noexcept;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    IDatabase* m_database;
    RequestHandlerFactory* m_handlerFactory;
    Communicator* m_communicator;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/
    
    Server();
};
