#pragma once

#include "../Managers/Question.h"
#include <string>
#include <vector>

class IDatabase
{
public:
	virtual ~IDatabase() = default;
	virtual bool open() = 0;
	virtual bool close() = 0;

	// Queries without callback
	virtual void addNewUser(const std::string& username, const std::string& password, const std::string& email) = 0;

	// Queries using callback
	virtual bool doesUserExist(const std::string& username) const = 0;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) const = 0;
	virtual std::vector<Question> getQuestions(const unsigned int numQuestions) const = 0;
	virtual float getPlayerAverageAnswerTime(const std::string& username) const = 0;
	virtual int getNumOfCorrectAnswers(const std::string& username) const = 0;
	virtual int getNumOfTotalAnswers(const std::string& username) const = 0;
	virtual int getNumOfPlayerGames(const std::string& username) const = 0;
	virtual float getPlayerScore(const std::string& username) const = 0;
	virtual std::vector<std::string> getHighScores() const = 0;
};
