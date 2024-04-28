#include "../../sqlite/sqlite3.h"
#include "../ServerDefenitions.h"
#include "SqliteDatabase.h"
#include <stdexcept>
#include <string>

using std::to_string;

bool SqliteDatabase::open()
{
	if (SQLITE_OK != sqlite3_open(DB_FILE_NAME.data(), &(this->_db)))
		throw std::runtime_error("Error while opening the DB: " + to_string(sqlite3_errcode(this->_db)));

	return true;
}

bool SqliteDatabase::close()
{
	if (this->_db != nullptr)
	{
		const bool isSuccess = sqlite3_close(this->_db) == SQLITE_OK;
		this->_db = nullptr;
		return isSuccess;
	}

	return true;
}

bool SqliteDatabase::doesUserExist(const std::string& username) const
{
	int numUsers = 0;
	const std::string query = "SELECT COUNT(USERNAME) FROM USERS WHERE USERNAME = " + username;

	this->runQuery(query, callbackInt, &numUsers);

	return numUsers != 0;
}

bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string_view& password) const
{
	std::string realPassword = "";
	const std::string query = "SELECT PASSWORD FROM USERS WHERE USERNAME = " + username;

	this->runQuery(query, callbackText, &realPassword);

	return password == realPassword;
}

void SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
	this->runQuery("INSERT (USERNAME, PASSWORD, MAIL) INTO USERS VALUES('" + username + "', '" + password + "', '" + email + "')");
}

// Helper functions

void SqliteDatabase::runQuery(const std::string_view& query) const
{
	runQuery(query, nullptr, nullptr);
}

void SqliteDatabase::runQuery(const std::string_view& query, const sqlite3_callback& callback, void* data) const
{
	char* sql_error_msg = nullptr;

	if (SQLITE_OK != sqlite3_exec(this->_db, query.data(), callback, data, &sql_error_msg))
	{
		const std::string err = sql_error_msg;
		sqlite3_free(sql_error_msg);
		throw std::runtime_error(err);
	}	
}

#pragma region CallbackFunctions

int SqliteDatabase::callbackInt(void* destination, int rows, char** data, char** columnsNames)
{
	if (rows == 1 && data[0] != nullptr)
	{
		*static_cast<int*>(destination) = atoi(data[0]);
		return 0;
	}
	return 1;
}

int SqliteDatabase::callbackText(void* destination, int rows, char** data, char** columnsNames)
{
	if (rows == 1 && data[0] != nullptr)
	{
		*static_cast<std::string*>(destination) = data[0];
		return 0;
	}
	return 1;
}
#pragma endregion
