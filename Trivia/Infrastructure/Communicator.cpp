#include "Communicator.h"
#include "Helper.h"
#include "JsonResponseSerializer.h"
#include "LoginRequestHandler.h"
#include "ServerDefenitions.h"
#include <iostream>
#include <string>
#include <thread>
#include <WinSock2.h>

using std::to_string;
constexpr uint16_t PORT = 7777;

// Singleton
Communicator* m_Communicator = nullptr;
std::mutex Communicator::m_mutex;


Communicator* Communicator::getInstance(RequestHandlerFactory* handlerFactory)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_Communicator == nullptr)
    {
        m_Communicator = new Communicator(handlerFactory);
    }
    return m_Communicator;
}

Communicator::Communicator(RequestHandlerFactory* handlerFactory)
    : m_handlerFactory(handlerFactory) {

    this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if (this->m_serverSocket == INVALID_SOCKET)
        throw std::runtime_error(__FUNCTION__ " - socket() err: " + to_string(WSAGetLastError()));

    this->bindAndListen();
}

Communicator::~Communicator() noexcept
{
    for (auto& client : this->m_clients)
        if (client.second != nullptr) delete client.second;
}

void Communicator::bindAndListen() const
{
    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET; // Must be AF_INET
    serverAddr.sin_port = htons(PORT); // Port that server will listen for
    serverAddr.sin_addr.s_addr = INADDR_ANY; // When there are few IPs for the machine. We will use always "INADDR_ANY"

    // Connects the socket and the configuration
    if (bind(this->m_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        throw std::runtime_error(__FUNCTION__ " - bind() err: " + to_string(WSAGetLastError()));

    // Start listening for incoming client requests
    if (listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
        throw std::runtime_error(__FUNCTION__ " - listen() err: " + to_string(WSAGetLastError()));

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
                throw std::runtime_error(__FUNCTION__ " - accept() err: " + to_string(WSAGetLastError()));

            std::cout << "Client accepted. Server and client can communicate\n";
            // Add client with LoginRequestHandler to map
            this->m_clients.emplace(clientSocket, new LoginRequestHandler(this->m_handlerFactory));

            // The function that handles the conversation with the client
            std::thread handlerThread(&Communicator::handleNewClient, this, clientSocket);
            handlerThread.detach();
        } while (true);
    }
    catch (const std::exception& e)
    {
        std::cerr << "\033[31;1m" << e.what() << "\033[0m\n";
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
            if (handler != nullptr || handler->isRequestRelevant(request))
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
                const buffer response = JsonResponseSerializer::serializeErrorResponse(ErrorResponse{"VERY ERRORY ERROR"});
                Helper::sendData(clientSocket, std::string(response.cbegin(), response.cend()));
                std::cout << "Operation NOT successful\n\n";
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "\033[31;1m" << e.what() << "\033[0m\n";
        }
    } while (true);
}
