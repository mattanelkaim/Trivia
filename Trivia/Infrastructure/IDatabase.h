#pragma once

#include "Question.h"
#include <cstdint>
#include <string>
#include <vector>

class IDatabase
{
public:
    virtual ~IDatabase() = default;
    
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
    virtual std::vector<std::string> getHighScores() const = 0;
};
