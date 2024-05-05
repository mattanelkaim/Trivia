#pragma once

#include <exception>
#include <string>

class IServerException : public std::exception
{
public:
	IServerException() = default;
	explicit IServerException(const std::string& err) : m_err(err) {}
	virtual const char* what() const override = 0;

protected:
	std::string m_err;
};

