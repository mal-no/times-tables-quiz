#include "question.hpp"

Question::Question() : number_(1), factor_(1) { }

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
Question::Question(int number, int factor)
    : number_(number), factor_(factor) { }

int Question::number()
{
    return number_;
}

int Question::factor()
{
    return factor_;
}

QString Question::questionBase()
{
    return TimesTables::questionBase;
}

bool Question::check(const int product)
{
    return factor_ * number_ == product;
}
