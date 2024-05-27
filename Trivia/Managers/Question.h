#pragma once

#include <string>
#include <vector>

class Question final
{
public:
    /*######################################
    ############# CONSTRUCTORS #############
    ######################################*/

    Question() = default;

    /**
    * @brief Constructs a new Question object.
    * 
    * @param question The question text.
    * @param answers A vector of possible answers. The correct answer should be the first one in the vector.
    */
    explicit Question(std::string question, const std::vector<std::string>& answers);

    /*######################################
    ############ PUBLIC METHODS ############
    ######################################*/

    const std::string& getQuestion() const noexcept;
    const std::vector<std::string>& getPossibleAnswers() const noexcept;
    const std::string& getCorrectAnswer() const noexcept; // The correct answer will always be the first one

    // Avoid shitty compiler warnings
    Question operator=(const Question& other) = delete;

private:
    /*######################################
    ################ MEMBERS ###############
    ######################################*/

    const std::string m_question;
    const std::vector<std::string> m_possibleAnswers;
};
