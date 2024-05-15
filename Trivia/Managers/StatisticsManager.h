#pragma once

#include "IDatabase.h"
#include <mutex>
#include <string>
#include <vector>

class StatisticsManager
{
public:
    std::vector<std::string> getHighScore() const;
    std::vector<std::string> getUserStatistics(const std::string& username) const;

    // Singleton
    StatisticsManager() = delete;
    StatisticsManager(const StatisticsManager& other) = delete;
    void operator=(const StatisticsManager& other) = delete;          
    static std::unique_ptr<StatisticsManager>& getInstance(IDatabase* db);
    ~StatisticsManager() noexcept = default;

private:
    IDatabase* m_database;    

    // Singleton
    explicit StatisticsManager(IDatabase* db);
    inline static std::unique_ptr<StatisticsManager> m_StatisticsManager = nullptr;
    inline static std::mutex m_mutex;
};
