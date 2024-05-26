#pragma once

#include "IDatabase.h"
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <memory>
#include <mutex>
#include <unordered_map>
#include <WinSock2.h>

/**
* @class Communicator
* @brief This class is responsible for handling communication between the server and its clients.
* 
* The Communicator class uses the Winsock API to create a server socket and listen for incoming client connections.
* When a client connects, the Communicator creates a new socket for communication with that client,
* and starts a new thread to handle the client's requests.
* 
* The Communicator uses a RequestHandlerFactory to create request handlers for handling the clients' requests. 
* Each client has its own request handler, which is stored in a map that associates each client's socket with its request handler.
* 
* The Communicator is a singleton, meaning there can be only one instance of it in the application. 
* You can get the singleton instance by calling the static getInstance() method.
* 
* @note This class is not copy-able or movable.
* 
* @example
* 
* IDatabase* db = new SqliteDatabase();
* Communicator* communicator = Communicator::getInstance(db);
* communicator->startHandleRequests();
*/
class Communicator final
{
public:
    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    /**
    * @brief Starts handling client requests.
    * 
    * This function accepts the client and creates a specific socket from the server to the client.
    * The process will not continue until a client connects to the server.
    * 
    * @throws std::runtime_error if accept() fails.
    */
    void startHandleRequests();

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    Communicator() = delete;
    Communicator(const Communicator& other) = delete;
    void operator=(const Communicator& other) = delete;
    static std::shared_ptr<Communicator> getInstance(IDatabase* db);
    ~Communicator() noexcept;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    std::shared_ptr<RequestHandlerFactory> m_handlerFactory;
    SOCKET m_serverSocket;
    std::unordered_map<SOCKET, IRequestHandler*> m_clients;

    /*######################################
    ############ PRIVATE METHODS ###########
    ######################################*/

    /**
    * @brief Binds the server socket to the specified IP address and port, and starts listening for incoming client requests.
    *
    * @throws std::runtime_error if there is an error binding the socket or starting the listening process.
    */
    void bindAndListen() const;

    /**
    * @brief Handles the communication with a new client.
    * 
    * This function runs in a separate thread for each client. It continuously reads data from the client's socket,
    * handles the client's requests, and sends responses back to the client. The function runs in an infinite loop
    * until an exception is thrown, indicating that the client has disconnected or an error has occurred.
    * 
    * @param clientSocket The socket connected to the client.
    */
    void handleNewClient(SOCKET clientSocket);

    void disconnectClient(SOCKET clientSocket) noexcept;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    explicit Communicator(IDatabase* db);
    inline static std::shared_ptr<Communicator> m_Communicator = nullptr;
    inline static std::mutex m_mutex;
};
