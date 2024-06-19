#include "Communicator.h"
#include "Helper.h"
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
#include <vector>
#include <winerror.h>
#include <WinSock2.h>

using std::to_string;


Communicator::Communicator() :
    m_serverSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
{
    if (!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!(this->m_serverSocket == INVALID_SOCKET))
        throw std::runtime_error(std::format("{}  - socket() err: ", __FUNCTION__) + to_string(WSAGetLastError()));

    this->bindAndListen();
}

Communicator::~Communicator()
{
    // Close server socket
    if (this->m_serverSocket != INVALID_SOCKET)
        closesocket(this->m_serverSocket);
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
            {
                if (WSAGetLastError() == WSAEINTR)
                    return; // Abort if interrupted by requestStop()

                // Else, throw exception to continue control flow
                throw std::runtime_error(std::format("{}  - accept() err: ", __FUNCTION__) + to_string(WSAGetLastError()));
            }

            std::cout << ANSI_GREEN << "Client accepted (" << clientSocket << ")\n" << ANSI_NORMAL;
            // Add client with LoginRequestHandler to map
            this->m_clients.emplace(clientSocket, new LoginRequestHandler);

            // The function that handles the conversation with the client
            std::thread handlerThread(&Communicator::handleNewClient, this, clientSocket);
            handlerThread.detach();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << ANSI_RED << Helper::formatError(__FUNCTION__, e.what()) << ANSI_NORMAL << '\n';
    }
}

void Communicator::handleNewClient(const SOCKET clientSocket)
{
    while (this->m_clients.contains(clientSocket))
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
            IRequestHandler*& handler = this->m_clients.at(clientSocket); // Must be a reference to exchange later

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
                Helper::sendData(clientSocket, JsonResponseSerializer::serializeResponse(ErrorResponse{"Request Isn't Relevant To The Current Handler"}));
                std::cout << "Operation NOT successful\n\n";
            }
        }
        catch (const ABORT_FLAG)
        {
            return; // Simply abort the thread
        }
        catch (const ServerException& e)
        {
            std::cerr << ANSI_RED << Helper::formatError(__FUNCTION__, e.what()) << ANSI_NORMAL << '\n';
            this->disconnectClient(clientSocket);
            std::cout << ANSI_BLUE << "Disconnected client socket " << clientSocket << "\n\n" << ANSI_NORMAL;
            return; // No need to handle disconnected client
        }
        catch (const std::exception& e)
        {
            std::cerr << ANSI_RED << Helper::formatError(__FUNCTION__, e.what()) << ANSI_NORMAL << '\n';
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

    closesocket(clientSocket);
}

// NOLINTNEXTLINE(bugprone-exception-escape) - ignore std::bad_alloc
void Communicator::disconnectAllClients() noexcept
{
    // IMPORTANT to backup the keys to a new vector, because we are deleting from the map!
    std::vector<SOCKET> disconnectedClients;
    disconnectedClients.reserve(this->m_clients.size());

    // Back up the client sockets
    for (const auto& client : this->m_clients)
    {
        disconnectedClients.push_back(client.first);
    }

    // Disconnect all clients
    for (const auto& clientSocket : std::move(disconnectedClients))
    {
        this->disconnectClient(clientSocket);
    }
}

// Singleton
Communicator& Communicator::getInstance()
{
    static Communicator instance; // This is thread-safe in C++11 and later
    return instance;
}
