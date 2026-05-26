#ifndef OTTQ_20250829_1809_INCLUDE
#define OTTQ_20250829_1809_INCLUDE

#include "abstract_tts_settings.hpp"
#include "locale_descriptor.hpp"
#include <QString>
#include <QSettings>

namespace Tts {

class Settings : public AbstractSettings
{
public:
    Settings();

    LocaleDescriptor loadLocaleSetting() override;
    bool loadAutoLocaleSetting() override;

    void saveLocaleSetting(const LocaleDescriptor &ld) override;
    void saveAutoLocaleSetting(const bool useAutoLocale) override;

private:
    QSettings settings_;

    AutoLocaleDescriptor<Tts::TranslationResources> autoLocale_;

    LocaleDescriptor &autoLocale() override;
};

namespace SettingsKeys {

static const QString language = "languageCode";
static const QString territory = "territoryCode";
static const QString autoLocale = "useAutoLocale";

} // namespace SettingsKeys

namespace SettingsDefaults {

static const Language::Code language = Language::any;
static const Territory::Code territory = Territory::ANY;
static const bool autoLocale = true;

} // namespace SettingsDefaults

} // namespace Tts

#endif // OTTQ_20250829_1809_INCLUDE
