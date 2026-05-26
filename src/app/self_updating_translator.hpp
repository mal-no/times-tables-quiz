#ifndef OTTQ_20260511_2050_INCLUDE
#define OTTQ_20260511_2050_INCLUDE

#include <tts/self_updating_translator.hpp>
#include <QObject>
#include <QQmlEngine>

class SelfUpdatingTranslator : public QObject
{
    Q_OBJECT
    // clang-format off
    Q_PROPERTY(QString localeName
               READ localeName
               NOTIFY localeNameChanged
               FINAL)
    // clang-format on
    QML_ELEMENT

public:
    explicit SelfUpdatingTranslator(QObject *parent = nullptr);

    QString localeName();

    Q_INVOKABLE QString translate(const QString &sourceText);

signals:
    void localeNameChanged();
    void error(const QString &msg);

private:
    Tts::SelfUpdatingTranslator<Tts::Translator> translator_;
};

#endif // OTTQ_20260511_2050_INCLUDE
