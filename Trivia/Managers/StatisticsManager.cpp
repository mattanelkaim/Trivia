#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(IDatabase* db) :
    m_database(db)
{}

std::vector<std::string> StatisticsManager::getHighScore() const
{
    return m_database->getHighScores();
}

std::vector<std::string> StatisticsManager::getUserStatistics(const std::string& username) const
{
    return // Returns an rvalue literal
    {
        std::to_string(m_database->getPlayerScore(username)),
        std::to_string(m_database->getNumOfPlayerGames(username)),
        std::to_string(m_database->getNumOfTotalAnswers(username)),
        std::to_string(m_database->getNumOfCorrectAnswers(username))
    };
}

// Singleton
std::unique_ptr<StatisticsManager>& StatisticsManager::getInstance(IDatabase* db)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_StatisticsManager == nullptr)
    {
        m_StatisticsManager = std::unique_ptr<StatisticsManager>(new StatisticsManager(db));
    }
    return m_StatisticsManager;
}
