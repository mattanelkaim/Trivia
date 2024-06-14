#pragma once

#include "IDatabase.h"
#include "Question.h"
#include "sqlite3.h"
#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <vector>

using safe_callback_ptr = int (*)(void*,int,char**, char**) noexcept; // sqlite3_callback noexcept

class SqliteDatabase final : public IDatabase
{
public:
    // Auto-closes the DB.
    ~SqliteDatabase() noexcept override;

    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    // @throws std::runtime_error If the database cannot be opened.
    bool openDB() override;
    bool closeDB() noexcept override;

    // @throws InvalidSQL
    void addNewUser(const std::string& username, const std::string& password, const std::string& email) override;

    // @throws InvalidSQL
    bool doesUserExist(const std::string& username) const override;

    // @throws InvalidSQL
    bool doesPasswordMatch(const std::string& username, const std::string& password) const override;

    // @throws InvalidSQL
    std::vector<Question> getQuestions(uint32_t numQuestions) const override;

    // @throws InvalidSQL
    float getPlayerAverageAnswerTime(const std::string& username) const override;

    // @throws InvalidSQL
    int getNumOfCorrectAnswers(const std::string& username) const override;

    // @throws InvalidSQL
    int getNumOfTotalAnswers(const std::string& username) const override;

    // @throws InvalidSQL
    int getNumOfPlayerGames(const std::string& username) const override;

    // @throws InvalidSQL
    float getPlayerScore(const std::string& username) const override;

    // @throws InvalidSQL
    std::map<std::string, double> getHighScores() const override;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    SqliteDatabase(const SqliteDatabase& other) = delete;
    void operator=(const SqliteDatabase& other) = delete;
    static SqliteDatabase& getInstance() noexcept;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    sqlite3* m_db{nullptr};

    /*######################################
    ############ PRIVATE METHODS ###########
    ######################################*/

    /**
    * Executes a given SQLite query on the DB (that doesn't fetch any data).
    * It's simply a convenience wrapper.
    * 
    * @param query The SQLite query to be executed.
    * 
    * @throws InvalidSQL
    */
    void runQuery(std::string_view query) const;

    /**
     * Executes a given SQLite query on the DB.
     * It uses a callback function that processes the results of a query (if it fetches data).
     * 
     * @param query The SQLite query to be executed.
     * @param callback A pointer to a *NOEXCEPT* callback function that's performed on EACH ROW fetched (can be nullptr).
     * @param data A pointer to user-defined data that will be passed to the callback function.
     *        Will store the procesed results of the query (can be nullptr).
     * 
     * @throws InvalidSQL
     */
    void runQuery(std::string_view query, safe_callback_ptr callback, void* data) const;

    /*######################################
    ########## CALLBACK FUNCTIONS ##########
    ######################################*/

    // columnNames is unused, but it's required by the sqlite3_callback signature.
    static int callbackInt(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept;
    static int callbackFloat(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept;
    static int callbackString(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept;
    static int callbackQuestionVector(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept;
    static int callbackStringDoubleMap(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    SqliteDatabase() noexcept = default;
};
