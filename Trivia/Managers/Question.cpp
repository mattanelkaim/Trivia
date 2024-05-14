#include "Question.h"

Question::Question(const std::string& question, const std::vector<std::string>& answers) :
    m_question(question),
    m_possibleAnswers(answers)
{}

const std::string& Question::getQuestion() const
{
    return this->m_question;
}

const std::vector<std::string>& Question::getPossibleAnswers() const
{
    return this->m_possibleAnswers;
}

const std::string& Question::getCorrectAnswer() const
{
    return this->m_possibleAnswers.front();
}
