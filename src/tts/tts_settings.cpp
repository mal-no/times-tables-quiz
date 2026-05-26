#include "tts_settings.hpp"

Tts::Settings::Settings() { }

auto Tts::Settings::loadLocaleSetting() -> LocaleDescriptor
{
    return LocaleDescriptor(
        settings_.value(SettingsKeys::language, SettingsDefaults::language)
            .value<Language::Code>(),
        settings_.value(SettingsKeys::territory, SettingsDefaults::territory)
            .value<Territory::Code>());
}

bool Tts::Settings::loadAutoLocaleSetting()
{
    return settings_
        .value(SettingsKeys::autoLocale, SettingsDefaults::autoLocale)
        .toBool();
}

void Tts::Settings::saveLocaleSetting(const LocaleDescriptor &ld)
{
    settings_.setValue(SettingsKeys::language, ld.language);
    settings_.setValue(SettingsKeys::territory, ld.territory);
}

void Tts::Settings::saveAutoLocaleSetting(const bool useAutoLocale)
{
    settings_.setValue(SettingsKeys::autoLocale, useAutoLocale);
}

auto Tts::Settings::autoLocale() -> LocaleDescriptor &
{
    autoLocale_.update();
    return autoLocale_;
}
