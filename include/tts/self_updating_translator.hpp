#ifndef OTTQ_20250829_1807_INCLUDE
#define OTTQ_20250829_1807_INCLUDE

#include "abstract_translator.hpp"
#include "abstract_tts_settings.hpp"
#include "tts_settings.hpp"
#if defined QT_TRANSLATOR
#  include "qt_translator.hpp"
#elif defined BOOST_TRANSLATOR
#  include "boost_translator.hpp"
#endif
#include <sstream>

namespace Tts {

template <typename T>
concept ExtendsTranslator = std::is_base_of_v<Tts::AbstractTranslator, T>;

#ifndef EXTENDS_RESOURCES
#  define EXTENDS_RESOURCES
template <typename TR>
concept ExtendsResources = std::is_base_of_v<Tts::TranslationResources, TR>;
// C++20
#endif // EXTENDS_RESOURCES

template <ExtendsTranslator TTranslator = Tts::Translator,
          ExtendsResources TTranslationResources = Tts::TranslationResources>
class SelfUpdatingTranslator : public TTranslator
{
private:
    std::shared_ptr<Tts::AbstractSettings> settings_;

    bool load(const std::string &filename) override
    {
        return TTranslator::load(filename);
    }

    void updateLocale()
    {
        auto localeDescriptor = TTranslator::localeDescriptor();
        auto settingsLocaleDescriptor =
            settings_->resolvedLocale().resourceKey();
        if (settingsLocaleDescriptor == localeDescriptor)
            return;
        localeDescriptor = std::move(settingsLocaleDescriptor);

        if (TTranslationResources::instance().get().empty())
            throw std::invalid_argument("Translation resources are empty.");

        if (!TTranslationResources::instance().get().contains(
                localeDescriptor)) {
            std::stringstream ss;
            ss << "No matching key in translation resources for locale "
                  "descriptor "
               << localeDescriptor << ".";
            throw std::invalid_argument(ss.str());
        }

        std::string resourcePath =
            TTranslationResources::instance().get().at(localeDescriptor);
        bool ok = TTranslator::load(resourcePath);

        if (!ok)
            throw std::runtime_error("Translation could not be loaded.");

        // TODO would it be possible to have separate qm files for tts?
    }

public:
    explicit SelfUpdatingTranslator(
        std::shared_ptr<Tts::AbstractSettings> settings =
            std::make_shared<Tts::Settings>())
        : settings_(settings)
    {
    }

    explicit SelfUpdatingTranslator(
        QObject *parent,
        std::shared_ptr<Tts::AbstractSettings> settings =
            std::make_shared<Tts::Settings>())
        : TTranslator(parent), settings_(settings)
    {
    }

#if defined BOOST_TRANSLATOR
    std::string translate(const boost::locale::basic_message<char> &sourceText)
#else
    std::string translate(const std::string &sourceText)
#endif
    {
        updateLocale();
        return TTranslator::translate(sourceText);
    }
};

} // namespace Tts

#endif // OTTQ_20250829_1807_INCLUDE
