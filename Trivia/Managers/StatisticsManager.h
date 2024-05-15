#pragma once

#include "IDatabase.h"
#include <string>
#include <vector>

class StatisticsManager
{
public:
    // Singelton
    StatisticsManager() = delete;
    StatisticsManager(const StatisticsManager& other) = delete;
    void operator=(const StatisticsManager& other) = delete;          
    static StatisticsManager* getInstance(IDatabase* db);

    std::vector<std::string> getHighScore() const;
    std::vector<std::string> getUserStatistics(const std::string& username) const;

private:
    IDatabase* m_database;    
    // Singleton
    explicit StatisticsManager(IDatabase* db);
    ~StatisticsManager() noexcept = default;
    inline static StatisticsManager* m_StatisticsManager = nullptr;
    inline static std::mutex m_mutex;
};
