#pragma once

#include <string>
#include <vector>

class Question 
{
public:
    // Constructors
    Question() = default;
    explicit Question(const std::string& question, const std::vector<std::string>& answers);

    const std::string& getQuestion() const;
    const std::vector<std::string>& getPossibleAnswers() const;
    const std::string& getCorrectAnswer() const; // The correct answer will always be the first one

    // Avoid shitty compiler warnings
    Question operator=(const Question& other) = delete;

private:
    const std::string m_question;
    const std::vector<std::string> m_possibleAnswers;
};
