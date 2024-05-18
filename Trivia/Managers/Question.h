#pragma once

#include <string>
#include <vector>

class Question final
{
public:
    // Constructors
    Question() = default;
    explicit Question(const std::string& question, const std::vector<std::string>& answers);

    const std::string& getQuestion() const noexcept;
    const std::vector<std::string>& getPossibleAnswers() const noexcept;
    const std::string& getCorrectAnswer() const noexcept; // The correct answer will always be the first one

    // Avoid shitty compiler warnings
    Question operator=(const Question& other) = delete;

private:
    const std::string m_question;
    const std::vector<std::string> m_possibleAnswers;
};
