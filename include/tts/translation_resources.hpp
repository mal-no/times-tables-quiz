#ifndef OTTQ_20251202_1917_INCLUDE
#define OTTQ_20251202_1917_INCLUDE

#include "locale_descriptor.hpp"
#include <vector>
#include <string>
#include <unordered_map>

template <>
struct std::hash<Tts::LocaleDescriptor>
{
    std::size_t operator()(const Tts::LocaleDescriptor &ld) const
    {
        return std::hash<int>()(ld.language)
            ^ std::hash<int>()(ld.territory << 1);
    }
};

// TODO should even for subdirectories the top namespace be OTTQ?

namespace Tts {

typedef std::unordered_map<Tts::LocaleDescriptor, std::string> ResourceMap;
typedef std::pair<Tts::LocaleDescriptor, std::string> ResourcePair;

// Use a struct instead of a namespace to be able to use it as type in
// templates. Then it can be replaced for testing.
struct TranslationResources
{
    static TranslationResources &instance();

    TranslationResources(const TranslationResources &) = delete;
    TranslationResources(const TranslationResources &&) = delete;
    TranslationResources &operator=(const TranslationResources &) = delete;
    TranslationResources &operator=(const TranslationResources &&) = delete;

    std::string path(const Tts::LocaleDescriptor &resourceKey);
    std::vector<Tts::LocaleDescriptor> locales();
    long index(const Tts::LocaleDescriptor &key);
    Tts::LocaleDescriptor locale(const long &index);

protected:
    TranslationResources();
    ~TranslationResources() { }

    void setLocales(const std::vector<Tts::LocaleDescriptor> &locales)
    {
        resources_.clear();
        for (const auto &l : locales)
            resources_.insert({ l, "" });
        locales_ = std::move(locales);
    }
    void addLocale(const LocaleDescriptor &locale)
    {
        resources_.insert({ locale, "" });
        locales_.push_back(locale);
    }
    void setResources(const ResourceMap &resources)
    {
        locales_.clear();
        for (const auto r : resources)
            locales_.push_back(r.first);
        resources_ = std::move(resources);
    }

private:
    ResourceMap resources_;
    std::vector<Tts::LocaleDescriptor> locales_;
};

} // namespace Tts

#endif // OTTQ_20251202_1917_INCLUDE
