#include "../Responses/JsonResponseSerializer.h"
#include "../ServerDefenitions.h"
#include "LoginRequestHandler.h"
#include <stdexcept>


bool LoginRequestHandler::isRequestRelevant(const RequestInfo& info)
{
    return info.id == LOGIN || info.id == SIGNUP;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& info)
{
    RequestResult result;

    switch (info.id)
    {
    case LOGIN:
        result.response = JsonResponseSerializer::serializeLoginResponse(LoginResponse{LOGIN});
        break;
    case SIGNUP:
        result.response = JsonResponseSerializer::serializeSignupResponse(SignupResponse{SIGNUP});
        break;
    default:
        throw std::runtime_error("RequestInfo is not login/signup!");
    }
    
    result.newHandler = nullptr; // TODO assign other handlers respectively

    return result;
}
