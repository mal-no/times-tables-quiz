import TtqApp
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: sRoot

    Column {
        padding: 10
        spacing: 10
        width: parent.width

        Label {
            opacity: 0.5
            text: qsTr("Text-to-Speech Settings")
        }

        GridLayout {
            columnSpacing: 10
            columns: 2
            rowSpacing: 10
            rows: 2

            Label {
                Layout.alignment: Qt.AlignVCenter
                Layout.column: 0
                Layout.row: 0
                opacity: switchTtsLocale.checked ? 1 : 0.5
                text: {
                    var l = autoLocale.nativeName;
                    return "%1\n(%2)".arg(l.language).arg(l.territory);
                }

                AutoLocale {
                    id: autoLocale
                }
            }

            Switch {
                id: switchTtsLocale

                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                Layout.column: 1
                Layout.row: 0
                checked: QuizSettings.useAutoLocale

                onToggled: QuizSettings.useAutoLocale = checked
            }

            ComboBox {
                Layout.column: 0
                Layout.columnSpan: 2
                Layout.fillWidth: true
                Layout.row: 1
                currentIndex: {
                    var language = QuizSettings.languageCode;
                    var territory = QuizSettings.territoryCode;
                    return languageListModel.index(language, territory);
                }
                enabled: !switchTtsLocale.checked

                model: LanguageListModel {
                    id: languageListModel
                }

                onActivated: {
                    var lc = languageListModel.languageCode(currentIndex);
                    var tc = languageListModel.territoryCode(currentIndex);
                    QuizSettings.languageCode = lc;
                    QuizSettings.territoryCode = tc;
                }
            }
        }

        Label {
            text: qsTr("Voice rate:")
        }

        SpinBox {
            from: -10
            textFromValue: function (value) {
                // FIXME causes a binding loop

                var rate = value / 10;
                return Number(rate).toLocaleString(Qt.locale("C"), 'f', 1);
            }
            to: 10
            value: QuizSettings.voiceRate * 10

            onValueChanged: QuizSettings.voiceRate = value / 10
        }
    }
}
