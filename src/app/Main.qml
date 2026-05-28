pragma ComponentBehavior: Bound
import TtqApp
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

ApplicationWindow {
    id: root

    property QuizConfigView quizConfigViewInst
    property QuizView quizViewInst

    title: qsTr("Times Tables Quiz")
    visible: true

    footer: ToolBar {
        ToolButton {
            id: btnNav

            anchors.fill: parent
            text: qsTr("Start")

            states: [
                State {
                    name: "back"

                    PropertyChanges {
                        btnMenu {
                            visible: false
                        }

                        btnNav {
                            text: qsTr("Go Back")

                            onClicked: {
                                state = "";
                                stack.pop();
                            }
                        }
                    }
                }
            ]

            onClicked: {
                state = "back";
                if (!root.quizViewInst.visible)
                    root.quizViewInst.visible = true;
                var p = {
                    "config": root.quizConfigViewInst.config
                };
                stack.push(root.quizViewInst, p);
            }
        }
    }
    header: ToolBar {
        id: tbHeader

        RowLayout {
            anchors.fill: parent

            Label {
                Layout.fillWidth: true
                Layout.leftMargin: 10
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignLeft
                opacity: 0.75
                text: root.title
                verticalAlignment: Qt.AlignVCenter
            }

            ToolButton {
                id: btnMenu

                property real leftMargin: parent.width - menu.width

                text: qsTr("Menu")

                onClicked: menu.popup(leftMargin, 0)
            }
        }
    }

    Component.onCompleted: {
        // Keep state of quiz view for faster navigation.
        var properties = {
            visible: false
        };
        const incubator = quizView.incubateObject(root, properties);
        if (incubator.status === Component.Ready) {
            quizViewInst = incubator.object;
            return;
        }
        incubator.onStatusChanged = function (status) {
            if (status == Component.Ready) {
                quizViewInst = incubator.object;
            }
        };
    }
    onClosing: close => {
        if (Qt.platform.os === "android" || Qt.platform.os === "ios") {
            if (stack.depth > 1) {
                close.accepted = false;
                btnNav.state = "";
                stack.pop();
            } else {
                close.accepted = true;
            }
        } else {
            close.accepted = true;
        }
    }

    Dialog {
        id: dlgAbout

        anchors.centerIn: parent
        modal: true
        standardButtons: Dialog.Ok
        title: qsTr("About")
    }

    Menu {
        id: menu

        MenuItem {
            text: qsTr("About")

            onClicked: dlgAbout.open()
        }

        MenuItem {
            text: qsTr("Settings")

            onClicked: {
                btnNav.state = "back";
                stack.push(settingsView);
            }
        }
    }

    StackView {
        id: stack

        anchors.fill: parent
        initialItem: setupView
    }

    Component {
        id: setupView

        QuizConfigView {
            Component.onCompleted: {
                root.quizConfigViewInst = this;
            }
        }
    }

    Component {
        id: quizView

        QuizView {
        }
    }

    Component {
        id: settingsView

        SettingsView {
        }
    }
}
