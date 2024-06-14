#include "Question.h"
#include <string>
#include <utility> // std::move
#include <vector>

Question::Question(std::string question, const std::vector<std::string>& answers) noexcept :
    m_question(std::move(question)),
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
