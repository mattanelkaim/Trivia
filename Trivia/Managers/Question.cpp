#include "Question.h"
#include <algorithm> // std::shuffle
#include <cstdint>
#include <random>
#include <string>
#include <utility> // std::move
#include <vector>

Question::Question(std::string question, std::vector<std::string> answers) noexcept :
    m_question(std::move(question)),
    m_possibleAnswers(std::move(answers)),
    m_correctAnswerId(0)
{
    const std::string correctAnswer = this->m_possibleAnswers.front(); // The correct answer is the first one (in DB)

    try
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(m_possibleAnswers.begin(), m_possibleAnswers.end(), gen);

        // Find the correct answer's new index
        for (uint8_t i = 0; const std::string& answer : m_possibleAnswers)
        {
            if (answer == correctAnswer)
            {
                m_correctAnswerId = i;
                return;
            }
            ++i;
        }
    }
    catch (...) // If shuffle fails, we will just use the original order
    {}
}

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
