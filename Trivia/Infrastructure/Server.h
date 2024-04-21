#pragma once

#include "Communicator.h"
#include "IDatabase.h"
#include "RequestHandlerFactory.h"
#include "./WSAInitializer.h"
#include <iostream>

class Server
{
private:
	// TODO cannot instantiate abstract IDatabase
	//IDatabase m_database;
	Communicator m_communicator;
	//RequestHandlerFactory m_handlerFactory;

public:
	void run();
};
