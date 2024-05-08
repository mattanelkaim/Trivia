#include "Communicator.h"
#include "Helper.h"
#include "IServerException.h"
#include "JsonResponseSerializer.h"
#include "LoginRequestHandler.h"
#include "ServerDefinitions.h"
#include <format>
#include <iostream>
#include <string>
#include <thread>
#include <WinSock2.h>

using std::to_string;

Communicator::Communicator(RequestHandlerFactory* handlerFactory) :
    m_handlerFactory(handlerFactory),
    m_serverSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
{
    if (this->m_serverSocket == INVALID_SOCKET)
        throw std::runtime_error(std::format("{}  - socket() err: ", __FUNCTION__) + to_string(WSAGetLastError()));

    this->bindAndListen();
}

Communicator::~Communicator() noexcept
{
    for (auto& [socket, handler] : this->m_clients)
        if (handler != nullptr) delete handler;
}

void Communicator::bindAndListen() const
{
    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET; // Must be AF_INET
    serverAddr.sin_port = htons(PORT); // Port that server will listen for
    serverAddr.sin_addr.s_addr = INADDR_ANY; // When there are few IPs for the machine. We will use always "INADDR_ANY"

    // Connects the socket and the configuration
    if (bind(this->m_serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
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
        do
        {
            // This accepts the client and create a specific socket from server to this client
            // The process will not continue until a client connects to the server
            SOCKET clientSocket = accept(m_serverSocket, NULL, NULL);
            if (clientSocket == INVALID_SOCKET)
                throw std::runtime_error(std::format("{}  - accept() err: ", __FUNCTION__) + to_string(WSAGetLastError()));

            std::cout << "Client accepted (" << clientSocket << "). Server and client can communicate\n";
            // Add client with LoginRequestHandler to map
            this->m_clients.emplace(clientSocket, new LoginRequestHandler(this->m_handlerFactory));

            // The function that handles the conversation with the client
            std::thread handlerThread(&Communicator::handleNewClient, this, clientSocket);
            handlerThread.detach();
        } while (true);
    }
    catch (const std::exception& e)
    {
        std::cerr << ANSI_RED << e.what() << ANSI_NORMAL << '\n';
    }
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
    do
    {
        try
        {
            // Read data from socket
            const RequestId code = static_cast<RequestId>(Helper::getCodeFromSocket(clientSocket));
            const std::string msg = Helper::getMessageFromSocket(clientSocket);

            // Initialize RequestInfo structure
            RequestInfo request{.id = code, .receivalTime = time(nullptr)};
            request.buffer.append_range(msg); // String to vector

            // Get current handler from clients map
            IRequestHandler* handler = this->m_clients.at(clientSocket);

            // Handle request
            if (handler != nullptr && handler->isRequestRelevant(request))
            {
                RequestResult result = handler->handleRequest(request); // Serialized

                // Update handler on map
                delete handler; // Done with old handler
                this->m_clients[clientSocket] = result.newHandler;

                Helper::sendData(clientSocket, std::string(result.response.cbegin(), result.response.cend()));
                std::cout << "Operation successful\n\n";
            }
            else
            {
                std::cout << "Handler is invalid!\n";
                const buffer response = JsonResponseSerializer::serializeErrorResponse(ErrorResponse{"VERY ERRORY ERROR"}); // MUST BE buffer AND NOT readonly_buffer
                Helper::sendData(clientSocket, std::string(response.cbegin(), response.cend()));
                std::cout << "Operation NOT successful\n\n";
            }
        }
        catch (const IServerException& e)
        {
            std::cerr << ANSI_RED << e.what() << ANSI_NORMAL << '\n';
            this->disconnectClient(clientSocket);
            std::cout << "Disconnected client socket " << clientSocket << "\n\n";
            return; // No need to handle disconnected client
        }
        catch (const std::exception& e)
        {
            std::cerr << ANSI_RED << e.what() << ANSI_NORMAL << '\n';
        }
    } while (true);
}

void Communicator::disconnectClient(const SOCKET clientSocket)
{
    IRequestHandler* handler = this->m_clients.at(clientSocket);
    if (handler != nullptr) delete handler;
    this->m_clients.erase(clientSocket);
}

// Singleton
Communicator* Communicator::getInstance(RequestHandlerFactory* handlerFactory)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_Communicator == nullptr)
    {
        m_Communicator = new Communicator(handlerFactory);
    }
    return m_Communicator;
}
