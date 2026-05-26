#ifndef OTTQ_20251222_1929_INCLUDE
#define OTTQ_20251222_1929_INCLUDE

#include "locale_descriptor.hpp"
#include "auto_locale_descriptor.hpp"
#include "translation_resources.hpp"

namespace Tts {

class AbstractSettings
{
private:
    LocaleDescriptor locale_;

protected:
    virtual LocaleDescriptor &autoLocale() = 0;

public:
    AbstractSettings() { }
    AbstractSettings(AbstractSettings const &) = default;
    AbstractSettings(AbstractSettings &&) noexcept = default;
    virtual ~AbstractSettings() = default;

    AbstractSettings &operator=(AbstractSettings const &) = default;
    AbstractSettings &operator=(AbstractSettings &&) noexcept = default;

    LocaleDescriptor &resolvedLocale()
    {
        locale_ = loadLocaleSetting();
        if (loadAutoLocaleSetting() || locale_.language <= Language::c) {
            return autoLocale();
        }

        return locale_;
    }

    virtual LocaleDescriptor loadLocaleSetting() = 0;
    virtual bool loadAutoLocaleSetting() = 0;

    virtual void saveLocaleSetting(const LocaleDescriptor &ld) = 0;
    virtual void saveAutoLocaleSetting(const bool useAutoLocale) = 0;
};

} // namespace Tts

#endif // OTTQ_20251222_1929_INCLUDE
