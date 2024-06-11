#include "SqliteDatabase.h"
#include "StatisticsManager.h"
#include <map>
#include <string>
#include <vector>


std::map<std::string, double> StatisticsManager::getHighScore() const
{
    return SqliteDatabase::getInstance().getHighScores();
}

std::vector<std::string> StatisticsManager::getUserStatistics(const std::string& username) const
{
    return // Returns an rvalue literal
    {
        std::to_string(SqliteDatabase::getInstance().getPlayerScore(username)),
        std::to_string(SqliteDatabase::getInstance().getNumOfPlayerGames(username)),
        std::to_string(SqliteDatabase::getInstance().getNumOfTotalAnswers(username)),
        std::to_string(SqliteDatabase::getInstance().getNumOfCorrectAnswers(username))
    };
}

// Singleton
StatisticsManager& StatisticsManager::getInstance() noexcept
{
    static StatisticsManager instance;
    return instance;
}
