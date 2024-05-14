#pragma once

#include "IDatabase.h"
#include <string>
#include <vector>

class StatisticsManager
{
public:
    StatisticsManager() = delete;
    StatisticsManager(IDatabase* db);

    std::vector<std::string> getHighScore() const;
    std::vector<std::string> getUserStatistics(const std::string& username) const;

private:
    IDatabase* m_database;
};
