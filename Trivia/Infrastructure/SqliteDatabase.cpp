#include "ServerDefinitions.h"
#include "sqlite3.h"
#include "SqliteDatabase.h"
#include <stdexcept>
#include <string>

using std::to_string;

SqliteDatabase::SqliteDatabase()
{
    this->open(); // TODO(mattan) construct tables in code
}

SqliteDatabase::~SqliteDatabase()
{
    this->close();
}

bool SqliteDatabase::open()
{
    if (SQLITE_OK != sqlite3_open(DB_FILE_NAME.data(), &(this->m_db)))
        throw std::runtime_error("Error while opening the DB: " + to_string(sqlite3_errcode(this->m_db)));

    return true;
}

bool SqliteDatabase::close()
{
    if (this->m_db != nullptr)
    {
        const bool isSuccess = sqlite3_close(this->m_db) == SQLITE_OK;
        this->m_db = nullptr;
        return isSuccess;
    }

    return true;
}

bool SqliteDatabase::doesUserExist(const std::string& username) const
{
    int numUsers = 0;
    const std::string query = "SELECT COUNT(username) FROM users WHERE username = '" + username + '\'';

    this->runQuery(query, callbackInt, &numUsers);

    return numUsers != 0;
}

// password could be a string_view, but unfortunately it does not match base class's method signature
bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password) const
{
    std::string realPassword = "";
    const std::string query = "SELECT password FROM users WHERE username = '" + username + '\'';

    this->runQuery(query, callbackText, &realPassword);

    return password == realPassword;
}

void SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
    this->runQuery("INSERT INTO users(username, password, email) VALUES('" + username + "', '" + password + "', '" + email + "')");
}

// Helper functions

void SqliteDatabase::runQuery(const std::string_view query) const
{
    runQuery(query, nullptr, nullptr);
}

void SqliteDatabase::runQuery(const std::string_view query, const safe_callback_ptr callback, void* data) const
{
    char* sql_error_msg = nullptr;

    if (SQLITE_OK != sqlite3_exec(this->m_db, query.data(), callback, data, &sql_error_msg))
    {
        const std::string err = sql_error_msg;
        sqlite3_free(sql_error_msg);
        throw std::runtime_error(err + " | ON query: '" + query.data() + '\'');
    }
}

#pragma region CallbackFunctions

int SqliteDatabase::callbackInt(void* destination, int rows, char** data, char**) noexcept
{
    if (rows == 1 && data[0] != nullptr)
    {
        *static_cast<int*>(destination) = atoi(data[0]);
        return 0;
    }
    return 1;
}

int SqliteDatabase::callbackText(void* destination, int rows, char** data, char**) noexcept
{
    if (rows == 1 && data[0] != nullptr)
    {
        *static_cast<std::string*>(destination) = data[0];
        return 0;
    }
    return 1;
}
#pragma endregion
