#ifndef OTTQ_20250829_1811_INCLUDE
#define OTTQ_20250829_1811_INCLUDE

#include "tts/tts_settings.hpp"
#include "language_name.hpp"
#include <QObject>
#include <qqml.h>
#include <QStringList>

class SettingsBackend : public QObject
{
    Q_OBJECT
    // clang-format off
    Q_PROPERTY(int languageIndex
               READ languageIndex
               WRITE setLanguageIndex
               NOTIFY languageIndexChanged
               FINAL)
    Q_PROPERTY(LanguageName autoLanguage
               READ autoLanguage
               NOTIFY autoLanguageChanged
               FINAL)
    // clang-format on
    QML_ELEMENT

public:
    explicit SettingsBackend(QObject *parent = nullptr);

    int languageIndex();
    bool useAutoTtsLanguage();
    LanguageName autoLanguage();

    void setLanguageIndex(const int index);
    void setUseAutoTtsLanguage(const bool useAutoTtsLanguage);

signals:
    void languageIndexChanged();
    void useAutoTtsLanguageChanged();
    void autoLanguageChanged();

private:
    Tts::Settings settings_;
};

#endif // OTTQ_20250829_1811_INCLUDE
