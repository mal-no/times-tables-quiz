#include "quiz.hpp"
#include <ranges>
#include <random>

Quiz::Quiz(QObject *parent) : QObject{ parent } { }

void Quiz::setup(const QuizConfiguration &qc)
{
    questions_.clear();

    if (qc.timesTables().empty()) {
        emit error("Tried to set up quiz, but tables are empty.");
        return;
    }

    generateQuestions(qc);
}

void Quiz::generateQuestions(const QuizConfiguration &qc)
{
    const auto range = qc.factorRange();
    const auto tables = qc.timesTables();

    const auto factors =
        std::ranges::iota_view{ range.first(), range.second() + 1 };
    for (const int number : tables)
        for (const int factor : factors)
            questions_.push_back(Question(number, factor));

    std::mt19937 gen{ std::random_device{}() };
    std::ranges::shuffle(questions_, gen);
    emit questionChanged();
    emit numQuestionsRemainingChanged();
}

Question Quiz::question()
{
    if (questions_.empty()) {
        emit error("Tried to get question, but questions are empty.");
        return Question(); // TODO how can I not return a default Question?
    }

    return questions_.back();
}

bool Quiz::next()
{
    questions_.pop_back();
    emit questionChanged();
    emit numQuestionsRemainingChanged();
    return !questions_.empty();
}

bool Quiz::answerIsCorrect(const int answer)
{
    if (questions_.empty()) {
        emit error("Tried to check answer, but questions are empty.");
        return false; // TODO should I really return a default value here?
    }

    return questions_.back().check(answer);
}

qsizetype Quiz::numQuestionsRemaining()
{
    // Returns -1 for empty questions, and 0 for "none left".
    // The last question in the list is the current question.
    // TODO should I instead just return the current question and save it
    // in qml for replay and checking?
    return questions_.size() - 1;
}
