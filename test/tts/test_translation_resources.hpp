#include "translation_resources.hpp"

namespace TtsTest {

struct TranslationResources : Tts::TranslationResources
{
    static TranslationResources &instance()
    {
        static TranslationResources tr;
        return tr;
    }

    TranslationResources(const TranslationResources &) = delete;
    TranslationResources(const TranslationResources &&) = delete;
    TranslationResources &operator=(const TranslationResources &) = delete;
    TranslationResources &operator=(const TranslationResources &&) = delete;

    void setLocales(const std::vector<Tts::LocaleDescriptor> &locales)
    {
        Tts::TranslationResources::setLocales(locales);
    }

    void addLocale(const Tts::LocaleDescriptor &locale)
    {
        Tts::TranslationResources::addLocale(locale);
    }

    void setResources(const Tts::ResourceMap &resources)
    {
        Tts::TranslationResources::setResources(resources);
    }

private:
    TranslationResources() { }
    ~TranslationResources() { }
};

} // namespace TtsTest
