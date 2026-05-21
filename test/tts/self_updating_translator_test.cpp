#include "test_translator.hpp"
#include "self_updating_translator.hpp"
#include "test_tts_settings.hpp"
#include <gtest/gtest.h>
#include <string>

TEST(TestTranslatorTest, TestTranslatorWorks)
{
    std::unordered_map<std::string, TtsTest::Locale> translations;
    translations.insert({ TtsTest::ResourcePaths::en, TtsTest::Locale::En });
    translations.insert({ TtsTest::ResourcePaths::de, TtsTest::Locale::De });

    TtsTest::Translator translator;
    translator.setup(translations, TtsTest::ResourcePaths::en);

    EXPECT_EQ(translator.localeDescriptor(),
              Tts::LocaleDescriptor(Tts::Language::en, Tts::Territory::ANY));
    EXPECT_EQ(translator.filePath(), TtsTest::ResourcePaths::en);
    std::string test = "test";
    std::string res = translator.translate(test);
    TtsTest::Translator::permutate(test, TtsTest::Locale::En);
    EXPECT_EQ(res, test);

    translator.load(TtsTest::ResourcePaths::de);

    EXPECT_EQ(translator.localeDescriptor(),
              Tts::LocaleDescriptor(Tts::Language::de, Tts::Territory::ANY));
    EXPECT_EQ(translator.filePath(), TtsTest::ResourcePaths::de);
    test = "test";
    res = translator.translate(test);
    TtsTest::Translator::permutate(test, TtsTest::Locale::De);
    EXPECT_EQ(res, test);
}

class SelfUpdatingTranslatorTest : public testing::Test
{
protected:
    SelfUpdatingTranslatorTest()
    {
        TtsTest::TranslationResources::instance().get().insert(
            { enAny_, TtsTest::ResourcePaths::en });
        TtsTest::TranslationResources::instance().get().insert(
            { deAny_, TtsTest::ResourcePaths::de });

        settings_ = std::make_shared<TtsTest::Settings>();
        settings_->saveLocaleSetting(enAny_);
        settings_->saveAutoLocaleSetting(false);
        translator_ = std::make_unique<
            Tts::SelfUpdatingTranslator<TtsTest::Translator,
                                        TtsTest::TranslationResources>>(
            settings_);

        Tts::Locale::setDefault(enUs_.language, enUs_.territory);

        std::unordered_map<std::string, TtsTest::Locale> translations;
        translations.insert(
            { TtsTest::ResourcePaths::en, TtsTest::Locale::En });
        translations.insert(
            { TtsTest::ResourcePaths::de, TtsTest::Locale::De });
        translator_->setup(translations, TtsTest::ResourcePaths::en);
    }

    // NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)
    std::shared_ptr<TtsTest::Settings> settings_;
    std::unique_ptr<Tts::SelfUpdatingTranslator<TtsTest::Translator,
                                                TtsTest::TranslationResources>>
        translator_;

    Tts::LocaleDescriptor enUs_{ Tts::Language::en, Tts::Territory::US };
    Tts::LocaleDescriptor deDe_{ Tts::Language::de, Tts::Territory::DE };
    Tts::LocaleDescriptor enAny_{ Tts::Language::en, Tts::Territory::ANY };
    Tts::LocaleDescriptor deAny_{ Tts::Language::de, Tts::Territory::ANY };

    std::string test1_ = "test1";
    std::string test2_ = "test2";
    // NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)
};

TEST_F(SelfUpdatingTranslatorTest, CatchesSettingsUpdate)
{
    Tts::LocaleDescriptor translatorLocale = translator_->localeDescriptor();
    EXPECT_EQ(translatorLocale, enAny_);

    std::string result = translator_->translate(test1_);
    TtsTest::Translator::permutate(test1_, TtsTest::Locale::En);
    EXPECT_EQ(result, test1_);

    // update settings with new locale
    settings_->saveLocaleSetting(deAny_);

    result = translator_->translate(test2_);
    TtsTest::Translator::permutate(test2_, TtsTest::Locale::De);
    EXPECT_EQ(result, test2_);

    translatorLocale = translator_->localeDescriptor();
    EXPECT_EQ(translatorLocale, deAny_);
}

TEST_F(SelfUpdatingTranslatorTest, CatchesSystemLocaleUpdate)
{
    settings_->saveAutoLocaleSetting(true);

    EXPECT_EQ(settings_->resolvedLocale(), enUs_);
    EXPECT_EQ(settings_->resolvedLocale().resourceKey(), enAny_);

    Tts::LocaleDescriptor translatorLocale = translator_->localeDescriptor();
    EXPECT_EQ(translatorLocale, enAny_);

    std::string result = translator_->translate(test1_);
    TtsTest::Translator::permutate(test1_, TtsTest::Locale::En);
    EXPECT_EQ(result, test1_);

    // update system locale
    Tts::Locale::setDefault(deDe_.language, deDe_.territory);
    EXPECT_EQ(settings_->resolvedLocale(), deDe_);
    EXPECT_EQ(settings_->resolvedLocale().resourceKey(), deAny_);

    result = translator_->translate(test2_);
    TtsTest::Translator::permutate(test2_, TtsTest::Locale::De);
    EXPECT_EQ(result, test2_);

    translatorLocale = translator_->localeDescriptor();
    EXPECT_EQ(translatorLocale, deAny_);
}
