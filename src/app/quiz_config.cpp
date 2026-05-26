#include "quiz_config.hpp"

QList<int> QuizConfiguration::timesTables() const
{
    return timesTables_;
}

QString QuizConfiguration::timesTablesStr() const
{
    QString timesTables;
    for (const int n : timesTables_)
        timesTables += QString::number(n) + ", ";

    if (timesTables.length() < 2)
        return "";
    return timesTables.first(timesTables.length() - 2);
}

FactorRange QuizConfiguration::factorRange() const
{
    return factorRange_;
}

void QuizConfiguration::setFactorRange(const FactorRange &fr)
{
    if (fr == factorRange_)
        return;

    factorRange_ = fr;
}

bool QuizConfiguration::addTimesTable(const int number)
{
    if (timesTables_.contains(number))
        return false;

    timesTables_.push_back(number);
    return true;
}

bool QuizConfiguration::remove(const int timesTableNumber)
{
    auto pos = timesTables_.indexOf(timesTableNumber);
    if (pos < 0)
        return false;

    timesTables_.removeAt(pos);
    return true;
}

bool QuizConfiguration::contains(const int number)
{
    return timesTables_.contains(number);
}

void QuizConfiguration::resetTimesTables()
{
    timesTables_.clear();
}
