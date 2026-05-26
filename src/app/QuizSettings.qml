pragma Singleton

import QtCore

Settings {
    property int languageCode: 1 // string, "c" for boost translator
    property int territoryCode: 0 // string, "" for boost translator
    property bool useAutoLocale: true
    property double voiceRate: 0.0
}
