#pragma once

#include "Question.h"
#include "ServerDefinitions.h"
#include <cstdint>
#include <map>
#include <string>
#include <vector>

class IDatabase
{
public:
    // @throws std::runtime_error
    virtual bool openDB() = 0;
    virtual bool closeDB() = 0;

    // @throws InvalidSQL
    virtual void addNewUser(const std::string& username, const std::string& password, const std::string& email) = 0;

    // @throws InvalidSQL
    virtual bool doesUserExist(const std::string& username) const = 0;
    
    // @throws InvalidSQL
    virtual bool doesPasswordMatch(const std::string& username, const std::string& password) const = 0;
    
    // @throws InvalidSQL
    virtual std::vector<Question> getQuestions(uint32_t numQuestions) const = 0;
    
    // @throws InvalidSQL
    virtual float getPlayerAverageAnswerTime(const std::string& username) const = 0;
    
    // @throws InvalidSQL
    virtual int getNumOfCorrectAnswers(const std::string& username) const = 0;
    
    // @throws InvalidSQL
    virtual int getNumOfTotalAnswers(const std::string& username) const = 0;
    
    // @throws InvalidSQL
    virtual int getNumOfPlayerGames(const std::string& username) const = 0;
    
    // @throws InvalidSQL
    virtual float getPlayerScore(const std::string& username) const = 0;
    
    // @throws InvalidSQL
    virtual std::map<std::string, double> getHighScores() const = 0;

    // @throws InvalidSQL
    virtual void updatePlayerStats(const std::string& username, const GameData& data) = 0;

    /*######################################
    ############### SINGLETON ##############
    ######################################*/

    IDatabase(const IDatabase& other) = delete;
    void operator=(const IDatabase& other) = delete;
    //static IDatabase& getInstance() noexcept {} // NEEDS TO BE IMPLEMENTED IN ALL INHERITING CLASSES
    virtual ~IDatabase() noexcept = default;

protected:
    IDatabase() = default;
};
