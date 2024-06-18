#pragma once

#pragma warning(disable: 4820) // Padding added after data member

#include <cstdint>
#include <string>
#include <vector>

class Question final
{
public:
    /*######################################
    ############# CONSTRUCTORS #############
    ######################################*/

    Question() noexcept = default;

    /**
    * Constructs a new Question object.
    * @param question The question text.
    * @param answers A vector of possible answers. The correct answer should be the first one in the vector.
    */
    Question(std::string question, std::vector<std::string> answers) noexcept;

    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    const std::string& getQuestion() const noexcept;
    const std::vector<std::string>& getPossibleAnswers() const noexcept;
    uint8_t getCorrectAnswerId() const noexcept; // The correct answer will always be the first one

    // Avoid shitty compiler warnings
    Question operator=(const Question& other) = delete;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    const std::string m_question;
    const std::vector<std::string> m_possibleAnswers;
    uint8_t m_correctAnswerId;
};
