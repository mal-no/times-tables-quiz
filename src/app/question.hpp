#ifndef OTTQ_20260501_1002_INCLUDE
#define OTTQ_20260501_1002_INCLUDE

#include <QObject>
#include <qqml.h>

namespace TimesTables {
static const char *const questionBase = "%1 times %2";
} // namespace TimesTables

class Question
{
    Q_GADGET
    // clang-format off
    Q_PROPERTY(int number
               READ number
               FINAL)
    Q_PROPERTY(int factor
               READ factor
               FINAL)
    Q_PROPERTY(QString questionBase
               READ questionBase
               FINAL)
    // clang-format on
    QML_VALUE_TYPE(question)
public:
    Question();
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    explicit Question(const int number, const int factor);

    int number();
    int factor();
    QString questionBase();

    Q_INVOKABLE bool check(const int product);

private:
    int number_;
    int factor_;
};

#endif // OTTQ_20260501_1002_INCLUDE
