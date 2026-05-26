#include "language_list_model.hpp"
#include "tts/translation_resources.hpp"
#include "tts/tts_settings.hpp"

LanguageListModel::LanguageListModel(QObject *parent) : QStringListModel(parent)
{
}

int LanguageListModel::rowCount(const QModelIndex &parent) const
{
    auto size = Tts::TranslationResources::instance().locales().size();
    if (size > INT_MAX)
        // TODO somehow show in ui
        return -1;

    // NOLINTNEXTLINE(*-narrowing-conversions)
    return size;
}

QHash<int, QByteArray> LanguageListModel::roleNames() const
{
    return { { 0, "language" } };
}

QVariant LanguageListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    std::vector<Tts::LocaleDescriptor> locales =
        Tts::TranslationResources::instance().locales();
    if (locales.size() < index.row())
        return QVariant();

    Tts::LocaleDescriptor ld = locales.at(index.row());
    Tts::Locale l(ld.language, ld.territory);

#ifdef QT_TRANSLATOR
    QLocale ql = static_cast<QLocale>(l);
#else
    QLocale ql(QString::fromStdString(l.name());
#endif

    return ql.nativeLanguageName();
}

size_t LanguageListModel::index(const Tts::Language::Code &languageCode,
                                const Tts::Territory::Code &territoryCode)
{
    Tts::LocaleDescriptor ld(languageCode, territoryCode);

    size_t index = Tts::TranslationResources::instance().index(ld);
    if (index < 0) {
        Tts::Settings settings;
        settings.saveAutoLocaleSetting(true);
        return 0;
    }
    return index;
}

Tts::Language::Code LanguageListModel::languageCode(const int &index)
{
    try {
        Tts::LocaleDescriptor ld =
            Tts::TranslationResources::instance().locale(index);
        return ld.language;
    } catch (const std::invalid_argument &e) {
        qInfo() << "Could not get language code:" << e.what();
    }
    return Tts::Language::c;
}

Tts::Territory::Code LanguageListModel::territoryCode(const int &index)
{
    try {
        Tts::LocaleDescriptor ld =
            Tts::TranslationResources::instance().locale(index);
        return ld.territory;
    } catch (const std::invalid_argument &e) {
        qInfo() << "Could not get territory code:" << e.what();
    }
    return Tts::Territory::ANY;
}
