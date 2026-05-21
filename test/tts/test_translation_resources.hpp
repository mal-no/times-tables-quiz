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

    Tts::ResourceMap &get() { return Tts::TranslationResources::resources(); }

private:
    TranslationResources() { }
    ~TranslationResources() { }
};

} // namespace TtsTest
