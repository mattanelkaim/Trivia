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
    const bool isSuccess = sqlite3_close(this->m_db) == SQLITE_OK;
    this->m_db = nullptr;
    return isSuccess;
}

bool SqliteDatabase::doesUserExist(const std::string& username) const
{
    const std::string query = "SELECT COUNT(*) FROM users WHERE username = '" + username + '\'';

    int numUsers;
    this->runQuery(query, callbackInt, &numUsers);

    return numUsers != 0;
}

// password could be a string_view, but unfortunately it does not match base class's method signature
bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password) const
{
    const std::string query = "SELECT password FROM users WHERE username = '" + username + '\'';

    std::string realPassword;
    this->runQuery(query, callbackText, &realPassword);

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

    float avgTime;
    this->runQuery(query, callbackFloat, &avgTime);

    return avgTime;
}

int SqliteDatabase::getNumOfCorrectAnswers(const std::string& username) const
{
    const std::string query = "SELECT num_correct FROM user_scores WHERE username = '" + username + '\'';

    int numCorrect;
    this->runQuery(query, callbackInt, &numCorrect);

    return numCorrect;
}

int SqliteDatabase::getNumOfTotalAnswers(const std::string& username) const
{
    const std::string query = "SELECT total_answers FROM user_scores WHERE username = '" + username + '\'';

    int numTotal;
    this->runQuery(query, callbackInt, &numTotal);

    return numTotal;
}

int SqliteDatabase::getNumOfPlayerGames(const std::string& username) const
{
    const std::string query = "SELECT num_games FROM user_scores WHERE username = '" + username + '\'';

    int totalGames;
    this->runQuery(query, callbackInt, &totalGames);

    return totalGames;
}

float SqliteDatabase::getPlayerScore(const std::string& username) const
{
    const std::string query = "SELECT score FROM user_scores WHERE username = '" + username + '\'';	

    float score;
    this->runQuery(query, callbackFloat, &score);

    return score;
}

std::vector<std::string> SqliteDatabase::getHighScores() const
{
    const std::string query = "SELECT name FROM user_scores ORDER BY score DESC LIMIT " + to_string(NUM_TOP_SCORES);

    std::vector<std::string> scores;
    this->runQuery(query, callbackStringVector, &scores);

    return scores;
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
    char* sqlErrorMsg = nullptr;

    if (sqlite3_exec(this->m_db, query.data(), callback, data, &sqlErrorMsg) != SQLITE_OK)
    {
        const std::string err = sqlErrorMsg;
        sqlite3_free(sqlErrorMsg);
        throw std::runtime_error(err + " | ON query: '" + query.data() + '\'');
    }
}

#pragma region CallbackFunctions
int SqliteDatabase::callbackInt(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept
{
    if (columns == 1 && data[0] != nullptr)
    {
        *static_cast<int*>(destination) = atoi(data[0]);
        return 0;
    }
    return 1;
}

int SqliteDatabase::callbackFloat(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept
{
    if (columns == 1 && data[0] != nullptr)
    {
        *static_cast<float*>(destination) = static_cast<float>(atof(data[0]));
        return 0;
    }
    return 1;
}

int SqliteDatabase::callbackText(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept
{
    if (columns == 1 && data[0] != nullptr)
    {
        *static_cast<std::string*>(destination) = data[0];
        return 0;
    }
    return 1;
}

int SqliteDatabase::callbackQuestionVector(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept
{
    try
    {
        auto dest = static_cast<std::vector<Question>*>(destination);

        const uint32_t num_columns = NUM_POSSIBLE_ANSWERS_PER_QUESTION + 1;

        for (int row = 0; row < columns; ++row)
        {
            std::vector<std::string> possible_answers(NUM_POSSIBLE_ANSWERS_PER_QUESTION);

            for (uint32_t i = 1; i <= NUM_POSSIBLE_ANSWERS_PER_QUESTION; ++i)
                possible_answers[i] = data[row * num_columns + i];

            dest->emplace_back(data[row], possible_answers);
        }
        return 0;
    }
    catch (...)
    {
        return 1;
    }
}

int SqliteDatabase::callbackStringVector(void* destination, int columns, char** data, [[maybe_unused]] char** columnsNames) noexcept
{
    try
    {
        auto dest = static_cast<std::vector<std::string>*>(destination);

        for (int i = 0; i < NUM_TOP_SCORES; ++i)
            dest->push_back(data[i]);

        return 0;
    }
    catch (...)
    {
        return 1;
    }
}
#pragma endregion
