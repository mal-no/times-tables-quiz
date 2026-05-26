#ifndef OTTQ_20251104_1759_INCLUDE
#define OTTQ_20251104_1759_INCLUDE

#include "factor_range.hpp"
#include <QObject>
#include <qqml.h>
#include <QList>

struct QuizConfiguration
{
    Q_GADGET
    // clang-format off
    Q_PROPERTY(QList<int> timesTables
               READ timesTables
               FINAL)
    Q_PROPERTY(QString timesTablesStr
               READ timesTablesStr
               FINAL)
    Q_PROPERTY(FactorRange factorRange
               READ factorRange
               WRITE setFactorRange
               FINAL)
    // clang-format on
    QML_VALUE_TYPE(quizConfiguration)

public:
    QList<int> timesTables() const;
    QString timesTablesStr() const;
    FactorRange factorRange() const;

    void setFactorRange(const FactorRange &fr);

    Q_INVOKABLE bool addTimesTable(const int number);
    Q_INVOKABLE bool remove(const int timesTableNumber);
    Q_INVOKABLE bool contains(const int number);
    Q_INVOKABLE void resetTimesTables();

private:
    QList<int> timesTables_;
    FactorRange factorRange_;
};

#endif // OTTQ_20251104_1759_INCLUDE
