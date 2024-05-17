#pragma once

#include "IDatabase.h"
#include "LoginManager.h"
#include <memory>
#include <mutex>

class LoginRequestHandler; // Double-circular-jerk-dependency-linkage mega-shit

class RequestHandlerFactory
{
public:
    LoginRequestHandler* createLoginRequestHandler();
    std::unique_ptr<LoginManager>& getLoginManager() noexcept;

    // Singleton
    RequestHandlerFactory() = delete;
    RequestHandlerFactory(RequestHandlerFactory& other) = delete;
    void operator=(const RequestHandlerFactory& other) = delete;
    static std::unique_ptr<RequestHandlerFactory>& getInstance(IDatabase* db);
    ~RequestHandlerFactory() = default;

private:
    IDatabase* m_database;
    std::unique_ptr<LoginManager>& m_loginManager;

    // Singleton
    explicit RequestHandlerFactory(IDatabase* db);
    inline static std::unique_ptr<RequestHandlerFactory> m_HandlerFactory = nullptr;
    inline static std::mutex m_mutex;
};
