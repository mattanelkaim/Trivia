#pragma once

#include "IDatabase.h"
#include "Question.h"
#include "sqlite3.h"
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

using safe_callback_ptr = int (*)(void*,int,char**, char**) noexcept; // sqlite3_callback noexcept

class SqliteDatabase final : public IDatabase
{
public:
    /**
    * @brief Constructs a new SqliteDatabase object.
    * 
    * This constructor opens the database.
    * 
    * @throws std::runtime_error If the database cannot be opened.
    */
    SqliteDatabase();

    /**
    * @brief Destroys the SqliteDatabase object.
    * 
    * This destructor closes the database.
    */
    ~SqliteDatabase() override;

    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    bool openDB() override;
    bool closeDB() override;

    // Queries without callback
    void addNewUser(const std::string& username, const std::string& password, const std::string& email) override;

    // Queries using callback
    bool doesUserExist(const std::string& username) const override;
    bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
    std::vector<Question> getQuestions(uint32_t numQuestions) const override;
    float getPlayerAverageAnswerTime(const std::string& username) const override;
    int getNumOfCorrectAnswers(const std::string& username) const override;	
    int getNumOfTotalAnswers(const std::string& username) const override;
    int getNumOfPlayerGames(const std::string& username) const override;
    float getPlayerScore(const std::string& username) const override;
    std::vector<std::string> getHighScores() const override;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    sqlite3* m_db{nullptr};

    /*######################################
    ############ PRIVATE METHODS ###########
    ######################################*/

    void runQuery(std::string_view query) const;
    void runQuery(std::string_view query, safe_callback_ptr callback, void* data) const;

    /*######################################
    ########## CALLBACK FUNCTIONS ##########
    ######################################*/

    static int callbackInt(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept;
    static int callbackFloat(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept;
    static int callbackString(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept;
    static int callbackStringVector(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept;
    static int callbackQuestionVector(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept;
};
