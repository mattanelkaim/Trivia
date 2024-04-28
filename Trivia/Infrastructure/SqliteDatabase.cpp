#include "../ServerDefenitions.h"
#include "SqliteDatabase.h"
#include <stdexcept>


bool SqliteDatabase::open()
{
	return sqlite3_open(DB_FILE_NAME, &this->_db) == SQLITE_OK;
}

bool SqliteDatabase::close()
{
	bool success = sqlite3_close(this->_db) == SQLITE_OK;
	this->_db = nullptr;
	return success;
}

bool SqliteDatabase::doesUserExist(const std::string& username) const
{
	int numUsers = 0;
	const std::string query = "SELECT COUNT(USERNAME) FROM USERS WHERE USERNAME = " + username + ';';

	this->runQuery(query.c_str(), this->callbackInt, &numUsers);

	return numUsers != 0;
}

bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	std::string realPassword = "";
	const std::string query = "SELECT PASSWORD FROM USERS WHERE USERNAME = " + username + ';';

	this->runQuery(query.c_str(), this->callbackInt, &realPassword);

	return password == realPassword;
}

void SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& mail)
{
	std::string query = "INSERT (USERNAME, PASSWORD, MAIL) INTO USERS VALUES ('" + username + "', '" + password + "', '" + mail + "');";

	this->runQuery(query.c_str(), this->callbackText, nullptr);
}

void SqliteDatabase::runQuery(const char* query, const sqlite3_callback& callback, void* data) const
{
	char* sql_error_msg = nullptr;

	sqlite3_exec(this->_db, query, nullptr, nullptr, &sql_error_msg);

	std::string error_msg_str;
	if (sql_error_msg)
	{
		const std::string err = sql_error_msg;
		delete sql_error_msg;
		throw std::runtime_error(err);
	}	
}

#pragma region CallbackFunctions

int SqliteDatabase::callbackInt(void* _data, int argc, char** argv, char** azColName)
{
	if (argc == 1 && argv[0] != nullptr)
	{
		*static_cast<int*>(_data) = atoi(argv[0]);
		return 0;
	}
	return 1;
}

int callbackText(void* _data, int argc, char** argv, char** azColName)
{
	if (argc == 1 && argv[0] != nullptr)
	{
		*static_cast<std::string*>(_data) = argv[0];
		return 0;
	}
	return 1;
}
#pragma endregion
