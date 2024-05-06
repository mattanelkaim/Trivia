#pragma once

#include "IDatabase.h"
#include "sqlite3.h"
#include <string>

using safe_callback_ptr = int (*)(void*,int,char**, char**) noexcept; // sqlite3_callback noexcept

class SqliteDatabase final : public IDatabase
{
public:
    SqliteDatabase();
    ~SqliteDatabase() override;

    bool open() override;
    bool close() override;

    // Queries
    bool doesUserExist(const std::string& username) const override;
    bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
    void addNewUser(const std::string& username, const std::string& password, const std::string& email) override;

private:
    sqlite3* _db;

    // Functions to run queries on the databases
    void runQuery(const std::string_view query) const;
    void runQuery(const std::string_view query, const safe_callback_ptr callback, void* data) const;

    // Callback functions
    static int callbackInt(void* destination, int rows, char** data, char** columnsNames) noexcept;
    static int callbackText(void* destination, int rows, char** data, char** columnsNames) noexcept;
};
