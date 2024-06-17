#pragma once

#include <map>
#include <string>
#include <vector>

class StatisticsManager final
{
public:
    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    /**
    * @brief Returns the high scores of all users.
    * 
    * This method retrieves the high scores from the database and returns them in a vector.
    * 
    * @return A vector of strings representing the high scores of all users.
    * @throws InvalidSQL
    */
    static std::map<std::string, double> getHighScore();

    /**
    * @brief Retrieves the statistics of the specified user from the database and returns them in a vector.
    * The vector contains the following statistics (in order): [1] The user's score, [2] The number of games the user has played,
    * [3] The total number of answers the user has given, [4] The number of correct answers the user has given.
    * 
    * @param username The username of the user whose statistics are to be retrieved.
    * @return A vector of strings representing the user's statistics.
    * @throws InvalidSQL
    */
    static std::vector<std::string> getUserStatistics(const std::string& username);

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    StatisticsManager(const StatisticsManager& other) = delete;
    void operator=(const StatisticsManager& other) = delete;          
    static StatisticsManager& getInstance() noexcept;
    ~StatisticsManager() noexcept = default;

private:
    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    StatisticsManager() noexcept = default;
};
