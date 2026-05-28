pragma ComponentBehavior: Bound
import TtqApp
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: suRoot

    property quizConfiguration config

    Column {
        anchors.centerIn: parent
        spacing: 10

        Item {
            height: timesTablesTitle.height
            width: parent.width

            SectionTitle {
                id: timesTablesTitle

                anchors.horizontalCenter: parent.horizontalCenter
                maxWidth: parent.width
                subtitle: {
                    var numbers = suRoot.config.timesTablesStr;
                    return numbers === "" ? qsTr("Add a number") : numbers;
                }
                title: qsTr("Times Tables") + ":"
            }

            MouseArea {
                anchors.left: timesTablesTitle.left
                anchors.top: timesTablesTitle.top
                height: timesTablesTitle.height
                width: timesTablesTitle.width

                onClicked: {
                    if (timesTablesTitle.elided)
                        dlgShowFullTimesTablesList.open();
                }
                onPressAndHold: {
                    if (suRoot.config.timesTables.length > 0)
                        dlgResetTimesTables.open();
                }
            }
        }

        Item {
            height: timesTableNumber.height + 10 + btnAddTimesTable.height
            width: parent.width

            SpinBox {
                id: timesTableNumber

                anchors.horizontalCenter: parent.horizontalCenter
                from: 1
                to: 100
                value: timesTableNumberDecimalStep.value

                onValueChanged: {
                    if (suRoot.config.contains(timesTableNumber.value))
                        btnAddTimesTable.state = "remove";
                    else
                        btnAddTimesTable.state = "";
                }
            }

            Slider {
                id: timesTableNumberDecimalStep

                anchors.bottom: btnAddTimesTable.bottom
                anchors.left: timesTableNumber.right
                anchors.leftMargin: 5
                anchors.top: timesTableNumber.top
                from: 90
                height: parent.height
                orientation: Qt.Vertical
                stepSize: 10
                to: 10
            }

            Button {
                id: btnAddTimesTable

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.left: timesTableNumber.left
                anchors.top: timesTableNumber.bottom
                anchors.topMargin: 10
                text: qsTr("Add")
                width: timesTableNumber.width

                states: [
                    State {
                        name: "remove"

                        PropertyChanges {
                            btnAddTimesTable {
                                text: qsTr("Remove")

                                onClicked: {
                                    // reset elided in case the text is now
                                    // short enough to not be elided.
                                    // elided will then be set again in the
                                    // title component when the text is changed.
                                    timesTablesTitle.resetElided();
                                    var n = timesTableNumber.value;
                                    if (suRoot.config.remove(n))
                                        state = "";
                                }
                            }
                        }
                    }
                ]

                onClicked: {
                    if (suRoot.config.addTimesTable(timesTableNumber.value))
                        state = "remove";
                }
            }
        }

        MenuSeparator {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
        }

        GridLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            columns: 3
            rowSpacing: 2
            rows: 2
            width: parent.width

            IncrementButton {
                firstNumberOfRange: true
            }

            DecrementButton {
                firstNumberOfRange: true
            }

            SectionTitle {
                id: factorRangeTitle

                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                Layout.column: 1
                Layout.row: 0
                Layout.rowSpan: 2
                subtitle: {
                    var first = factorRange.intFirst;
                    var second = factorRange.intSecond;
                    return "[%1, %2]".arg(first).arg(second);
                }
                title: qsTr("Factors:")
            }

            IncrementButton {
                firstNumberOfRange: false
            }

            DecrementButton {
                firstNumberOfRange: false
            }
        }

        RangeSlider {
            id: factorRange

            property int intFirst: Math.round(first.value)
            property int intSecond: Math.round(second.value)

            anchors.horizontalCenter: parent.horizontalCenter
            first.value: suRoot.config.factorRange.first
            from: 1
            second.value: suRoot.config.factorRange.second
            snapMode: RangeSlider.SnapAlways
            stepSize: 1
            to: 100

            first.onMoved: suRoot.config.factorRange.first = intFirst
            second.onMoved: suRoot.config.factorRange.second = intSecond
        }

        MenuSeparator {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter

            RadioButton {
                text: qsTr("Text")

                onCheckedChanged: suRoot.config.showQuestion = checked
            }

            RadioButton {
                checked: true
                text: qsTr("Audio")
                onCheckedChanged: suRoot.config.showQuestion = !checked
            }
        }
    }

    Dialog {
        id: dlgResetTimesTables

        anchors.centerIn: parent
        margins: 10
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel
        title: qsTr("Resetting Times Tables")

        onAccepted: {
            suRoot.config.resetTimesTables();
            btnAddTimesTable.state = "";
        }

        Label {
            elide: Text.ElideRight
            text: qsTr("The list of times tables will be cleared. Are you sure?")
            width: parent.width
            wrapMode: Text.WordWrap
        }
    }

    Dialog {
        id: dlgShowFullTimesTablesList

        anchors.centerIn: parent
        margins: 10
        modal: true
        standardButtons: Dialog.Ok
        title: qsTr("Times Tables")
        // This dialog needs the width to be set explicitly, to fix a binding
        // loop for the dynamically set text of the label.
        width: Math.min(parent.width - leftMargin - rightMargin, 300)

        Label {
            text: suRoot.config.timesTablesStr
            width: parent.width
            wrapMode: Text.WordWrap
        }
    }

    component DecrementButton: StepButton {
        Layout.row: 1
        text: "-"

        onClicked: {
            if (firstNumberOfRange) {
                factorRange.first.decrease();
                factorRange.first.moved();
            } else {
                factorRange.second.decrease();
                factorRange.second.moved();
            }
        }
    }
    component IncrementButton: StepButton {
        Layout.row: 0
        text: "+"

        onClicked: {
            if (firstNumberOfRange) {
                factorRange.first.increase();
                factorRange.first.moved();
            } else {
                factorRange.second.increase();
                factorRange.second.moved();
            }
        }
    }
    component SectionTitle: Column {
        id: stRoot

        property bool elided: false
        property int maxWidth
        required property string subtitle
        required property string title

        function resetElided() {
            if (elided)
                elided = false;
        }

        spacing: 2

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#888"
            text: stRoot.title
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            elide: Text.ElideMiddle
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            text: stRoot.subtitle

            Component.onCompleted: {
                if (stRoot.maxWidth !== 0)
                    width = stRoot.maxWidth;
            }

            // set elided
            onLineLaidOut: line => {
                // Only assign the property when changed to not emit signals
                // too often.
                // Also, avoid using a zero width comparison in case maxWidth
                // is not used.
                if (!stRoot.elided && stRoot.maxWidth !== 0) {
                    var tmp = line.implicitWidth > stRoot.maxWidth;
                    if (tmp)
                        stRoot.elided = tmp;
                }
            }
        }
    }
    component StepButton: RoundButton {
        // An enum inside an inline component is not usable (Qt 6.10), so it
        // would be defined at the top of the root component, far away from its
        // usage when using qmlformat.
        // Also, the lines become much longer.
        required property bool firstNumberOfRange // first/second of RangeSlider

        Layout.alignment: firstNumberOfRange ? Qt.AlignLeft : Qt.AlignRight
        Layout.column: firstNumberOfRange ? 0 : 2
        Layout.preferredHeight: factorRangeTitle.height / 2
        flat: true
    }
}
