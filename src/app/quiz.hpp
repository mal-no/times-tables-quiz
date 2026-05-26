#ifndef OTTQ_20260501_0759_INCLUDE
#define OTTQ_20260501_0759_INCLUDE

#include "quiz_config.hpp"
#include "question.hpp"
#include <QObject>
#include <qqml.h>

class Quiz : public QObject
{
    Q_OBJECT
    // clang-format off
    Q_PROPERTY(qsizetype numQuestionsRemaining
               READ numQuestionsRemaining
               NOTIFY numQuestionsRemainingChanged
               FINAL)
    // clang-format on
    QML_ELEMENT

public:
    explicit Quiz(QObject *parent = nullptr);

    Q_INVOKABLE void setup(const QuizConfiguration &qc);
    Q_INVOKABLE Question question();
    Q_INVOKABLE bool next();
    Q_INVOKABLE bool answerIsCorrect(const int answer);

    qsizetype numQuestionsRemaining();

signals:
    void questionChanged();
    void numQuestionsRemainingChanged();
    void error(const QString &msg);

private:
    void generateQuestions(const QuizConfiguration &qc);

    QList<Question> questions_;
};

#endif // OTTQ_20260501_0759_INCLUDE
