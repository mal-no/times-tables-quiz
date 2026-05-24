#ifndef OTTQ_20260524_2024_INCLUDE
#define OTTQ_20260524_2024_INCLUDE

#include "tts/auto_locale.hpp"
#include "language_name.hpp"
#include <QObject>
#include <QQmlEngine>

class AutoLocale : public QObject
{
    Q_OBJECT
    // clang-format off
    Q_PROPERTY(LanguageName nativeName
               READ nativeName
               NOTIFY nativeNameChanged
               FINAL)
    // clang-format on
    QML_ELEMENT

public:
    explicit AutoLocale(QObject *parent = nullptr);

    LanguageName nativeName();

signals:
    void nativeNameChanged();

private:
    Tts::AutoLocale<Tts::TranslationResources> autoLocale_;
};

#endif // OTTQ_20260524_2024_INCLUDE
