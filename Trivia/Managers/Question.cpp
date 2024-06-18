#include "Question.h"
#include <cstdint>
#include <string>
#include <utility> // std::move
#include <vector>

Question::Question(std::string question, std::vector<std::string> answers) noexcept :
    m_question(std::move(question)),
    m_possibleAnswers(std::move(answers)),
    m_correctAnswerId(0)
{}

const std::string& Question::getQuestion() const noexcept
{
    return this->m_question;
}

const std::vector<std::string>& Question::getPossibleAnswers() const noexcept
{
    return this->m_possibleAnswers;
}

uint8_t Question::getCorrectAnswerId() const noexcept
{
    return m_correctAnswerId;
}
