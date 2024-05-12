#pragma once

#include <vector>
#include <string>

class Question 
{
public:
	//C'tor
	Question() = default;
	Question(const std::string& question, const std::vector<std::string>& answers);
	inline const std::string& getQuestion() const;
	inline const std::vector<std::string>& getPossibleAnswers() const;
	inline const std::string& getCorrectAnswer() const; // the correct answer will always be the first one

private:
	const std::string m_question;
	const std::vector<std::string> m_possibleAnswers;
};