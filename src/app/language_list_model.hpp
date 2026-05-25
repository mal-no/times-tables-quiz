#ifndef OTTQ_20260519_1934_INCLUDE
#define OTTQ_20260519_1934_INCLUDE

#include "tts/qt_language.hpp"
#include "tts/qt_territory.hpp"
#include <QObject>
#include <qqml.h>
#include <QStringListModel>

class LanguageListModel : public QStringListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    LanguageListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    Q_INVOKABLE size_t index(const Tts::Language::Code &languageCode,
                             const Tts::Territory::Code &territoryCode);
    Q_INVOKABLE Tts::Language::Code languageCode(const int &index);
    Q_INVOKABLE Tts::Territory::Code territoryCode(const int &index);
};

#endif // OTTQ_20260519_1934_INCLUDE
