#pragma once

#include <string>
#include <vector>

class Question 
{
public:
    // Constructors
    Question() = delete;
    Question(const std::string& question, const std::vector<std::string>& answers);

    const std::string& getQuestion() const;
    const std::vector<std::string>& getPossibleAnswers() const;
    const std::string& getCorrectAnswer() const; // The correct answer will always be the first one

private:
    const std::string m_question;
    const std::vector<std::string> m_possibleAnswers;
};
