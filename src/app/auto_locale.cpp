#include "auto_locale.hpp"

AutoLocale::AutoLocale(QObject *parent) : QObject{ parent } { }

LanguageName AutoLocale::nativeName()
{
#ifdef QT_TRANSLATOR
    QLocale l = static_cast<QLocale>(autoLocale_);
#else
    QLocale l(QString::fromStdString(autoLocale_.name()));
#endif
    LanguageName languageName(l.nativeLanguageName(), l.nativeTerritoryName());
    return languageName;
}
