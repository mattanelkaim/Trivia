#pragma once

#include "IDatabase.h"
#include <string>
#include <vector>
#include <mutex>
#include <memory>

class StatisticsManager
{
friend std::unique_ptr<StatisticsManager>;

public:
    // Singelton
    ~StatisticsManager() noexcept = default;
    StatisticsManager() = delete;
    StatisticsManager(const StatisticsManager& other) = delete;
    void operator=(const StatisticsManager& other) = delete;          
    static std::unique_ptr<StatisticsManager>& getInstance(IDatabase* db);

    std::vector<std::string> getHighScore() const;
    std::vector<std::string> getUserStatistics(const std::string& username) const;

private:
    IDatabase* m_database;    
    // Singleton
    explicit StatisticsManager(IDatabase* db);
    inline static std::unique_ptr<StatisticsManager> m_StatisticsManager = nullptr;
    inline static std::mutex m_mutex;
};
