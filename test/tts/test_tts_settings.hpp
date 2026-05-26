#ifndef OTTQ_20251222_1939_INCLUDE
#define OTTQ_20251222_1939_INCLUDE

#include "abstract_tts_settings.hpp"
#include "locale_descriptor.hpp"
#include "test_translation_resources.hpp"
#include "auto_locale_descriptor.hpp"

namespace TtsTest {

class Settings : public Tts::AbstractSettings
{
private:
    Tts::LocaleDescriptor savedLocale_;
    bool savedUseAutoLocale_{ false };

    Tts::AutoLocaleDescriptor<TtsTest::TranslationResources> autoLocale_;

    Tts::LocaleDescriptor &autoLocale() override
    {
        autoLocale_.update();
        return autoLocale_;
    }

public:
    Settings() { }

    Tts::LocaleDescriptor loadLocaleSetting() override { return savedLocale_; }
    bool loadAutoLocaleSetting() override { return savedUseAutoLocale_; }

    void saveLocaleSetting(const Tts::LocaleDescriptor &ld) override
    {
        savedLocale_ = std::move(ld);
    }
    void saveAutoLocaleSetting(const bool useAutoLocale) override
    {
        savedUseAutoLocale_ = useAutoLocale;
    }
};

} // namespace TtsTest

#endif // OTTQ_20251222_1939_INCLUDE
