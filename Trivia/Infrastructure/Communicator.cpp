#include "../Responses/JsonResponseSerializer.h"
#include "../ServerDefenitions.h"
#include "Communicator.h"
#include "Helper.h"
#include <iostream>
#include <string>
#include <thread>
#include <WinSock2.h>

using std::to_string;
constexpr uint16_t PORT = 7777;


Communicator::Communicator()
{
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
            this->m_clients.emplace(clientSocket, new LoginRequestHandler);

            // The function that handles the conversation with the client
            std::thread handlerThread(&Communicator::handleNewClient, this, clientSocket);
            handlerThread.detach();
        } while (true);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
    try
    {
        // Read data from socket
        const RequestId code = static_cast<RequestId>(Helper::getCodeFromSocket(clientSocket));
        const std::string msg = Helper::getMessageFromSocket(clientSocket);

        // Initialize RequestInfo structure
        RequestInfo request{.id = code, .receivalTime = time(nullptr)};
        request.buffer.append_range(msg);

        // Get current handler from clients map
        LoginRequestHandler* handler = (LoginRequestHandler*)(this->m_clients.at(clientSocket));
        
        if (handler->isRequestRelevant(request))
        {
            RequestResult result = handler->handleRequest(request); // Serialized

            // Update handler on map
            delete handler; // Finished with old handler
            this->m_clients[clientSocket] = result.newHandler;

            Helper::sendData(clientSocket, std::string(result.response.cbegin(), result.response.cend()));
            std::cout << "Login successful\n";
        }
        else
        {
            const buffer response = JsonResponseSerializer::serializeErrorResponse(ErrorResponse{}); // TODO(mattan) use response struct?
            Helper::sendData(clientSocket, std::string(response.cbegin(), response.cend()));
            std::cout << "Login NOT successful\n";
        }

    }
    catch (const std::exception& e)
    {

        std::cerr << e.what() << '\n';
    }
}
