#include "factor_range.hpp"

FactorRange::FactorRange() : first_(1), second_(20) { }

int FactorRange::first() const
{
    return first_;
}

int FactorRange::second() const
{
    return second_;
}

void FactorRange::setFirst(const int first)
{
    if (first_ == first)
        return;
    first_ = first;
}

void FactorRange::setSecond(const int second)
{
    if (second_ == second)
        return;
    second_ = second;
}

bool FactorRange::operator==(const FactorRange &fr)
{
    return first_ == fr.first() && second_ == fr.second();
}
