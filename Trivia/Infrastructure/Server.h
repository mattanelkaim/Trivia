#pragma once

#include "Communicator.h"
#include "IDatabase.h"
#include "RequestHandlerFactory.h"
#include <memory>
#include <mutex>

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
    static std::shared_ptr<Server> getInstance();
    ~Server() noexcept;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    IDatabase* m_database;
    std::shared_ptr<RequestHandlerFactory> m_handlerFactory;
    std::shared_ptr<Communicator> m_communicator;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/
    
    Server();
    inline static std::shared_ptr<Server> m_Server = nullptr;
    inline static std::mutex m_mutex;
};
