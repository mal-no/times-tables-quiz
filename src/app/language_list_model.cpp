#include "language_list_model.hpp"
#include "tts/translation_resources.hpp"

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
