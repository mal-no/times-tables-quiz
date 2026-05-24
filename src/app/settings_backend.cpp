#include "settings_backend.hpp"
#include "tts/auto_locale.hpp"
#include "tts/translation_resources.hpp"
#include <QString>
#include <QtLogging>
#include <stdexcept>
#include <algorithm>

SettingsBackend::SettingsBackend(QObject *parent) : QObject(parent) { }

QStringList SettingsBackend::languages()
{
    static QStringList languageNames;
    if (languageNames.size() > 0)
        return languageNames;

    std::ranges::transform( // C++20
        Tts::TranslationResources::instance().locales(),
        std::back_inserter(languageNames),
        [](const Tts::LocaleDescriptor &ld) -> QString {
            Tts::Locale l(ld.language, ld.territory);
#ifdef QT_TRANSLATOR
            QLocale ql = static_cast<QLocale>(l);
#else
            QLocale ql(QString::fromStdString(l.name()));
#endif
            return ql.nativeLanguageName();
        });

    return languageNames;
}

int SettingsBackend::languageIndex()
{
    Tts::LocaleDescriptor localeKey = settings_.loadLocaleSetting();
    // Because I don't expect large translation resource lists, the type
    // of index_ doesn't need to be long:
    // NOLINTNEXTLINE(*-narrowing-conversions)
    int index = Tts::TranslationResources::instance().index(localeKey);
    if (index < 0) {
        setUseAutoTtsLanguage(true);
        return 0;
    }
    return index;
}

bool SettingsBackend::useAutoTtsLanguage()
{
    return settings_.loadAutoLocaleSetting();
}

auto SettingsBackend::autoLanguage() -> LanguageName
{
    Tts::AutoLocale al;
#ifdef QT_TRANSLATOR
    QLocale l = static_cast<QLocale>(al);
#else
    QLocale l(QString::fromStdString(al.name()));
#endif
    LanguageName languageName(l.nativeLanguageName(), l.nativeTerritoryName());
    return languageName;
}

double SettingsBackend::voiceRate()
{
    return settings_.loadVoiceRateSetting();
}

void SettingsBackend::setLanguageIndex(const int index)
{
    if (languageIndex() == index)
        return;

    try {
        Tts::LocaleDescriptor ld =
            Tts::TranslationResources::instance().locale(index);
        settings_.saveLocaleSetting(ld);
        emit languageIndexChanged();
    } catch (const std::invalid_argument &e) {
        qInfo() << "Could not set index:" << e.what();
    }
}

void SettingsBackend::setUseAutoTtsLanguage(const bool useAutoTtsLanguage)
{
    if (SettingsBackend::useAutoTtsLanguage() == useAutoTtsLanguage)
        return;

    settings_.saveAutoLocaleSetting(useAutoTtsLanguage);
    emit useAutoTtsLanguageChanged();
}

void SettingsBackend::setVoiceRate(const double rate)
{
    if (SettingsBackend::voiceRate() == rate)
        return;

    settings_.saveVoiceRateSetting(rate);
    emit voiceRateChanged();
}
