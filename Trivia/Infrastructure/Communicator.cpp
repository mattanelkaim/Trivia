#include "Communicator.h"
#include "Helper.h"
#include "IDatabase.h"
#include "JsonResponseSerializer.h"
#include "LoginRequestHandler.h"
#include "ServerDefinitions.h"
#include "ServerException.h"
#include <ctime>
#include <exception>
#include <format>
#include <inaddr.h> // s_addr macro
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility> // std::exchange
#include <WinSock2.h>

using std::to_string;


Communicator::Communicator(IDatabase* db) :
    m_handlerFactory(RequestHandlerFactory::getInstance(db)),
    m_serverSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
{
    if (!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!(this->m_serverSocket == INVALID_SOCKET))
        throw std::runtime_error(std::format("{}  - socket() err: ", __FUNCTION__) + to_string(WSAGetLastError()));

    this->bindAndListen();
}

Communicator::~Communicator() noexcept
{
    for (auto& [client, handler] : this->m_clients)
    {
        this->disconnectClient(client);
        delete handler;
    }
}

void Communicator::bindAndListen() const
{
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET; // Must be AF_INET
    serverAddr.sin_port = htons(PORT); // Port that server will listen for
    serverAddr.sin_addr.s_addr = INADDR_ANY; // When there are few IPs for the machine. We will use always "INADDR_ANY"

    // Connects the socket and the configuration
    if (bind(this->m_serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
        throw std::runtime_error(std::format("{}  - bind() err: ", __FUNCTION__) + to_string(WSAGetLastError()));

    // Start listening for incoming client requests
    if (listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
        throw std::runtime_error(std::format("{}  - listen() err: ", __FUNCTION__) + to_string(WSAGetLastError()));

    std::cout << "Listening on port " << PORT << "...\n\n";
}

void Communicator::startHandleRequests()
{
    try
    {
        while (true)
        {
            // This accepts the client and create a specific socket from server to this client
            // The process will not continue until a client connects to the server
            const SOCKET clientSocket = accept(m_serverSocket, nullptr, nullptr);
            if (clientSocket == INVALID_SOCKET)
                throw std::runtime_error(std::format("{}  - accept() err: ", __FUNCTION__) + to_string(WSAGetLastError()));

            std::cout << ANSI_GREEN << "Client accepted (" << clientSocket << ")\n" << ANSI_NORMAL;
            // Add client with LoginRequestHandler to map
            this->m_clients.emplace(clientSocket, new LoginRequestHandler(this->m_handlerFactory));

            // The function that handles the conversation with the client
            std::thread handlerThread(&Communicator::handleNewClient, this, clientSocket);
            handlerThread.detach();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << ANSI_RED << e.what() << ANSI_NORMAL << '\n';
    }
}

void Communicator::handleNewClient(const SOCKET clientSocket)
{
    while (true)
    {
        try
        {
            // Read data from socket
            const auto code = static_cast<RequestId>(Helper::getCodeFromSocket(clientSocket));
            // msg is a temporary object that will be moved to the RequestInfo structure
            std::string msg = Helper::getMessageFromSocket(clientSocket);

            // Initialize RequestInfo structure
            const RequestInfo request{.id = code, .receivalTime = std::time(nullptr),
                                      .buffer{std::from_range, std::move(msg)}}; // String to vector

            // Get current handler from clients map
            IRequestHandler*& handler = this->m_clients.at(clientSocket);

            // Handle request if valid
            if (handler != nullptr && handler->isRequestRelevant(request)) [[likely]]
            {
                const RequestResult result = handler->handleRequest(request); // Serialized

                // Update handler in map
                if (result.newHandler != nullptr) [[unlikely]]
                    delete std::exchange(handler, result.newHandler); // Done with old handler

                // Send response back to client
                Helper::sendData(clientSocket, result.response);
                std::cout << "Operation successful\n\n";
            }
            else [[unlikely]]
            {
                // Serialize an error response and send it to the client
                Helper::sendData(clientSocket, JsonResponseSerializer::serializeResponse(ErrorResponse{"VERY ERRORY ERROR"}));
                std::cout << "Operation NOT successful\n\n";
            }
        }
        catch (const ServerException& e)
        {
            std::cerr << ANSI_RED << e.what() << ANSI_NORMAL << '\n';
            this->disconnectClient(clientSocket);
            std::cout << ANSI_BLUE << "Disconnected client socket " << clientSocket << "\n\n" << ANSI_NORMAL;
            return; // No need to handle disconnected client
        }
        catch (const std::exception& e)
        {
            std::cerr << ANSI_RED << e.what() << ANSI_NORMAL << '\n';
        }
    }
}

void Communicator::disconnectClient(const SOCKET clientSocket) noexcept
{
    // Send a goodbye message to the client if possible
    try
    {
        Helper::sendData(clientSocket, JsonResponseSerializer::serializeResponse(ErrorResponse{"You got disconnected! L bozo"}));
    }
    catch (const std::runtime_error&)
    {} // Ignore if socket is invalid

    // Delete client from map and free handler memory
    const auto& client = this->m_clients.find(clientSocket);
    if (client != this->m_clients.cend()) [[likely]]
    {
        delete client->second; // Delete handler
        this->m_clients.erase(clientSocket);
    }
}

// Singleton
Communicator* Communicator::getInstance(IDatabase* db)
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    if (m_Communicator == nullptr)
    {
        m_Communicator = std::unique_ptr<Communicator>(new Communicator(db));
    }
    return m_Communicator.get();
}
