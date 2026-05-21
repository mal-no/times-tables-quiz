#ifndef OTTQ_20250829_1806_INCLUDE
#define OTTQ_20250829_1806_INCLUDE

#include "locale_descriptor.hpp"
#include "translation_resources.hpp"
#if defined QT_TRANSLATOR
#  include "qt_locale.hpp"
#elif defined BOOST_TRANSLATOR
#  include "boost_locale.hpp"
#endif
#include <optional>
#include <ostream>

namespace Tts {

#ifndef EXTENDS_RESOURCES
#  define EXTENDS_RESOURCES
template <typename T>
concept ExtendsResources = std::is_base_of_v<Tts::TranslationResources, T>;
// C++20
#endif // EXTENDS_RESOURCES

template <ExtendsResources TTranslationResources = Tts::TranslationResources>
struct AutoLocaleDescriptor : public LocaleDescriptor
{
    AutoLocaleDescriptor() { set(); }

    void update()
    {
        Tts::Locale updatedSystem;
        if (updatedSystem.language() != language
            || updatedSystem.territory() != territory)
            set();
    }

    Tts::LocaleDescriptor resourceKey() const override { return resourceKey_; }

private:
    Tts::LocaleDescriptor resourceKey_;

    void set()
    {
        Tts::LocaleDescriptor system = Tts::LocaleDescriptor(Tts::Locale());

        std::optional<Tts::LocaleDescriptor> fallback;

        // T::resources() is not saved in a static local variable on purpose:
        // it can change for unit tests.
        // It can already have a state in the resources getter to avoid
        // rebuilding the list on every call.
        const Tts::ResourceMap resources =
            TTranslationResources::instance().get();

        // An empty translation resource list should not be possible. If the
        // translations are id-based, there is not even a default English
        // translation available, so allowing the c-locale as fallback would
        // not make sense.
        if (resources.size() == 0)
            throw std::invalid_argument(
                "No translation resource files found for auto locale");

        for (Tts::ResourcePair r : resources) {
            Tts::LocaleDescriptor resource = r.first;

            if (resource == system) {
                language = system.language;
                territory = system.territory;
                resourceKey_ = resource;
                return;
            }

            // The preferred fallback would be finding a translation resource
            // with a matching language. Of those translation resources, take
            // the first.
            if (!fallback.has_value() && resource.language == system.language) {
                fallback = system;
                resourceKey_ = resource;
            }
        }

        // If there is no translation available for the system language, use the
        // first translation resource as 'automatic' tts locale.
        // It can still be changed manually anyways.
        if (!fallback.has_value()) {
            fallback = resources.begin()->first;
            resourceKey_ = *fallback;
        }

        language = fallback->language;
        territory = fallback->territory;
    }
};

template <
    Tts::ExtendsResources TTranslationResources = Tts::TranslationResources>
inline std::ostream &
operator<<(std::ostream &os,
           const Tts::AutoLocaleDescriptor<TTranslationResources> &ld)
{
    return os << static_cast<Tts::LocaleDescriptor>(ld) << ", key "
              << ld.resourceKey();
}

} // namespace Tts

#endif // OTTQ_20250829_1806_INCLUDE
