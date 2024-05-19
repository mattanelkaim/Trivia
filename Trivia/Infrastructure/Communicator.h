#pragma once

#include "IDatabase.h"
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <memory>
#include <mutex>
#include <unordered_map>
#include <WinSock2.h>

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
    Communicator(Communicator& other) = delete;
    void operator=(const Communicator& other) = delete;
    static Communicator* getInstance(IDatabase* db);
    ~Communicator() noexcept;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    RequestHandlerFactory& m_handlerFactory;
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

    /**
    * @brief Disconnects a client from the server.
    * 
    * @param clientSocket The socket of the client to disconnect.
    */
    void disconnectClient(SOCKET clientSocket) noexcept;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    explicit Communicator(IDatabase* db);
    inline static std::unique_ptr<Communicator> m_Communicator = nullptr;
    inline static std::mutex m_mutex;
};
