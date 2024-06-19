#include "InvalidSQL.h"
#include "Question.h"
#include "ServerDefinitions.h"
#include "sqlite3.h"
#include "SqliteDatabase.h"
#include <algorithm> // std::copy
#include <cstdint>
#include <cstdlib> // std::atoi, std::atof
#include <iterator> // std::back_inserter
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

using std::to_string;


SqliteDatabase::~SqliteDatabase() noexcept
{
    this->closeDB();
}

SqliteDatabase& SqliteDatabase::getInstance() noexcept
{
    static SqliteDatabase instance; // This is thread-safe in C++11 and later
    return instance;
}

bool SqliteDatabase::openDB()
{
    if ((sqlite3_open(DB_FILE_NAME.data(), &(this->m_db))) != SQLITE_OK)
        throw std::runtime_error("Error while opening the DB: " + to_string(sqlite3_errcode(this->m_db)));

    return true;
    // TODO(mattan) construct tables in code
}

bool SqliteDatabase::closeDB() noexcept
{
    const bool isSuccess = sqlite3_close(this->m_db) == SQLITE_OK;
    this->m_db = nullptr; // Could use std::exchange with line above
    return isSuccess;
}

bool SqliteDatabase::doesUserExist(const std::string& username) const
{
    const std::string query = "SELECT COUNT(*) FROM users WHERE username = '" + username + '\'';

    int numUsers{};
    this->runQuery(query, callbackInt, &numUsers);

    return numUsers != 0;
}

// password could be a string_view, but unfortunately it does not match base class's method signature
bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password) const
{
    const std::string query = "SELECT password FROM users WHERE username = '" + username + '\'';

    std::string realPassword;
    this->runQuery(query, callbackString, &realPassword);

    return password == realPassword;
}

std::vector<Question> SqliteDatabase::getQuestions(const uint32_t numQuestions) const
{
    const std::string query = "SELECT question, correct, ans1, ans2, ans3 FROM questions LIMIT " + to_string(numQuestions);

    std::vector<Question> questions;
    this->runQuery(query, callbackQuestionVector, &questions);

    return questions;
}

float SqliteDatabase::getPlayerAverageAnswerTime(const std::string& username) const
{
    const std::string query = "SELECT avg_time FROM user_scores WHERE username = '" + username + '\'';

    float avgTime{};
    this->runQuery(query, callbackFloat, &avgTime);

    return avgTime;
}

int SqliteDatabase::getNumOfCorrectAnswers(const std::string& username) const
{
    const std::string query = "SELECT num_correct FROM user_scores WHERE username = '" + username + '\'';

    int numCorrect{};
    this->runQuery(query, callbackInt, &numCorrect);

    return numCorrect;
}

int SqliteDatabase::getNumOfTotalAnswers(const std::string& username) const
{
    const std::string query = "SELECT total_answers FROM user_scores WHERE username = '" + username + '\'';

    int numTotal{};
    this->runQuery(query, callbackInt, &numTotal);

    return numTotal;
}

int SqliteDatabase::getNumOfPlayerGames(const std::string& username) const
{
    const std::string query = "SELECT num_games FROM user_scores WHERE username = '" + username + '\'';

    int totalGames{};
    this->runQuery(query, callbackInt, &totalGames);

    return totalGames;
}

float SqliteDatabase::getPlayerScore(const std::string& username) const
{
    const std::string query = "SELECT score FROM user_scores WHERE username = '" + username + '\'';	

    float score{};
    this->runQuery(query, callbackFloat, &score);

    return score;
}

std::map<std::string, double> SqliteDatabase::getHighScores() const
{
    const std::string query = "SELECT username, score FROM user_scores ORDER BY score DESC LIMIT " + to_string(NUM_TOP_SCORES);

    std::map<std::string, double> highScores;
    this->runQuery(query, callbackStringDoubleMap, &highScores);

    return highScores;
}

void SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
    this->runQuery("INSERT INTO users(username, password, email) VALUES('" + username + "', '" + password + "', '" + email + "')");
}

void SqliteDatabase::updatePlayerStats(const std::string& username, const GameData& data)
{
    const auto newTotalAnswers = data.correctAnswerCount + data.wrongAnswerCount; // new total

    const std::string query = "UPDATE user_scores SET num_correct = num_correct + " + to_string(data.correctAnswerCount) +
        ", num_wrong = num_wrong + " + to_string(data.wrongAnswerCount) +
        // new avg = (last avg * last total + new avg * new total) / (new total + last total)
        ", avg_time = (avg_time*total_answers+" + to_string(data.totalAnswerTime) + ") / (" + to_string(newTotalAnswers) + "+total_answers)"
        ", num_games = num_games + 1 WHERE username = '" + username + '\'';

    this->runQuery(query);
}


// Helper functions


void SqliteDatabase::runQuery(const std::string_view query) const
{
    runQuery(query, nullptr, nullptr);
}

void SqliteDatabase::runQuery(const std::string_view query, const safe_callback_ptr callback, void* data) const
{
    char* sqlErrorMsg = nullptr; // Will be set by sqlite3_exec() if an error occurs

    if (sqlite3_exec(this->m_db, query.data(), callback, data, &sqlErrorMsg) != SQLITE_OK)
    {
        const std::string err = sqlErrorMsg;
        sqlite3_free(sqlErrorMsg);
        throw InvalidSQL(err + " | ON query: '" + query.data() + '\'');
    }
}


#pragma region CallbackFunctions
#pragma warning(push)
#pragma warning(disable : 26481 26461 26447) // const for data param & don't use poitner arithmetic & noexcept violation
// NOLINTBEGIN(clang-diagnostic-unsafe-buffer-usage)

int SqliteDatabase::callbackInt(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept
{
    if (columns != 1 || data[0] == nullptr)
        return 1;

    *static_cast<int*>(destination) = std::stol(data[0]);
    return 0;
}

int SqliteDatabase::callbackFloat(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept
{
    if (columns != 1 || data[0] == nullptr)
        return 1;

    *static_cast<float*>(destination) = std::stof(data[0]);
    return 0;
}

int SqliteDatabase::callbackString(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept
{
    if (columns != 1 || data[0] == nullptr)
        return 1;
    
    try
    {
        *static_cast<std::string*>(destination) = data[0];
        return 0;
    }
    catch (...) // std::bad_alloc
    {
        return 1;
    }
}

// Used in getQuestions()
int SqliteDatabase::callbackQuestionVector(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept
{
    if (columns != NUM_POSSIBLE_ANSWERS_PER_QUESTION + 1)
        return 1; // Error

    try
    {
        std::vector<std::string> possibleAnswers(NUM_POSSIBLE_ANSWERS_PER_QUESTION); // Pre-reserve size
        std::copy(data + 1, data + columns, std::back_inserter(possibleAnswers)); // Skip index 0 (which is the actual question)

        static_cast<std::vector<Question>*>(destination)->emplace_back(data[0], possibleAnswers); // Construct with question string & answers
        return 0;
    }
    catch (...) // Callbacks must be noexcept
    {
        return 1;
    }
}

// Used in getHighScores()
int SqliteDatabase::callbackStringDoubleMap(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept
{
    if (columns != 2) // 2 = number of members in a pair
        return 1; // Error

    try
    {
        static_cast<std::map<std::string, double>*>(destination)->emplace(data[0], std::stof(data[1])); // Construct with question string & answers
        return 0;
    }
    catch (...) // Callbacks must be noexcept
    {
        return 1;
    }
}

// NOLINTEND(clang-diagnostic-unsafe-buffer-usage)
#pragma warning(pop)
#pragma endregion
