#include "Question.h"

Question::Question(const std::string& question, const std::vector<const std::string>& answers) :
    m_question(question),
    m_possibleAnswers(answers)
{}

inline const std::string& Question::getQuestion() const
{
    return this->m_question;
}

inline const std::vector<const std::string>& Question::getPossibleAnswers() const
{
    return this->m_possibleAnswers;
}

inline const std::string& Question::getCorrectAnswer() const
{
    return this->m_possibleAnswers.front();
}
