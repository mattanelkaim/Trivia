#include "Question.h"
#include <string>
#include <vector>

Question::Question(const std::string& question, const std::vector<std::string>& answers) :
    m_question(question),
    m_possibleAnswers(answers)
{}

const std::string& Question::getQuestion() const noexcept
{
    return this->m_question;
}

const std::vector<std::string>& Question::getPossibleAnswers() const noexcept
{
    return this->m_possibleAnswers;
}

const std::string& Question::getCorrectAnswer() const noexcept
{
    return this->m_possibleAnswers.front();
}
