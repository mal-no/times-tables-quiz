#include "self_updating_translator.hpp"

SelfUpdatingTranslator::SelfUpdatingTranslator(QObject *parent)
    : QObject(parent), translator_(Tts::SelfUpdatingTranslator(this))
{
}

QString SelfUpdatingTranslator::localeName()
{
    return QString::fromStdString(translator_.locale().name());
}

QString SelfUpdatingTranslator::translate(const QString &sourceText)
{
    QString res;

    try {
        res = QString::fromStdString(
            translator_.translate(sourceText.toStdString()));
        // TODO only emit if the locale actually changed
        emit localeNameChanged();
    } catch (const std::invalid_argument &e) {
        qCritical("Translation setup failed: %s", e.what());
        emit error(e.what());
    } catch (const std::runtime_error &e) {
        qCritical("Translation setup failed: %s", e.what());
        emit error(e.what());
    }

    return res;
}
