#pragma once

/**
* @class WSAInitializer
* @brief This class is used to initialize Winsock.
* 
* Winsock is a technical specification that defines how Windows network software should access network services, especially TCP/IP.
* It's necessary to initialize Winsock before using any network functionality in your Windows-based application.
* 
* This class automatically initializes Winsock when you create an instance of it, and cleans up Winsock when the instance is destroyed.
* 
* @note Only one instance of this class is required per application, not per thread. However, if your application creates threads that use Winsock, 
* you should create the instance before creating any threads, to avoid race conditions.
* 
* @example
* 
* int main()
* {
*     WSAInitializer wsaInit;
*     // Now you can use network functionality...
* 
*     // Winsock is automatically cleaned up when wsaInit goes out of scope
*     return 0;
* }
*/
class WSAInitializer final
{
public:
    /**
    * @brief Constructs a new WSAInitializer object.
    * 
    * This constructor initializes Winsock. If the initialization fails, it throws a std::runtime_error exception.
    * The exception's message includes the error code returned by WSAGetLastError.
    *
    * @throws std::runtime_error Thrown when the initialization of Winsock fails.
    */
    WSAInitializer();

    /**
    * @brief Destroys the WSAInitializer object.
    * 
    * This destructor cleans up Winsock. If the cleanup fails, it catches and ignores the exception.
    */
    ~WSAInitializer() noexcept;
};
