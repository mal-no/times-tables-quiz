import OttqApp
import QtQuick
import QtQuick.Controls.Basic
import QtTextToSpeech
import QtQml

FocusScope {
    id: qRoot

    property quizConfiguration config

    signal showLocaleError

    function check(answer: string) {
        if (answer === "")
            return;

        if (quiz.answerIsCorrect(parseInt(answer)))
            nextQuestion();
    }

    function nextQuestion() {
        if (quiz.next()) {
            qRoot.sayQuestion();
        } else {
            // Let last question fade out instead of stopping it.
            qRoot.state = "completed";
        }
    }

    function sayQuestion() {
        var q = quiz.question();
        var qStrBase = translator.translate(q.ttsQuestionBase);
        var qStr = qStrBase.arg(q.number).arg(q.factor);
        // Stop current question and start next right away instead of
        // enqueueing. This way the quiz is more snappy.
        if (tts.state === TextToSpeech.Speaking) {
            tts.stop();
            tts.say(qStr);
        } else {
            // Enqueue in case tts is not ready for reasons other than
            // currently speaking.
            tts.enqueue(qStr);
        }
    }

    state: "setting-up"

    states: [
        State {
            name: "unavailable"

            PropertyChanges {
                answerInput {
                    enabled: false
                    focus: false
                    placeholderText: qsTr("Unavailable")
                    text: ""
                }

                progressBarTtsLoading {
                    visible: false
                }

                labelNumQuestionsRemaining {
                    visible: false
                }

                btnReplay {
                    visible: false
                }
            }
        },
        State {
            extend: "unavailable"
            name: "setting-up"

            PropertyChanges {
                answerInput {
                    placeholderText: qsTr("Setting Up")
                }

                progressBarTtsLoading {
                    visible: true
                }

                labelLocale {
                    text: ""
                }
            }
        },
        State {
            extend: "setting-up"
            name: "tts-loading"

            PropertyChanges {
                answerInput {
                    placeholderText: qsTr("Text-to-Speech Loading")
                }
            }
        },
        State {
            extend: "setting-up"
            name: "tts-synthesizing"

            PropertyChanges {
                answerInput {
                    placeholderText: qsTr("Text-to-Speech Synthesizing")
                }
            }
        },
        State {
            name: "available"

            PropertyChanges {
                answerInput {
                    enabled: true
                    focus: true
                    placeholderText: qsTr("Result")
                    text: ""
                }

                progressBarTtsLoading {
                    visible: false
                }

                labelNumQuestionsRemaining {
                    visible: true
                }

                btnReplay {
                    visible: true
                }
            }
        },
        State {
            extend: "unavailable"
            name: "completed"

            PropertyChanges {
                answerInput {
                    placeholderText: qsTr("Completed")
                    text: ""
                }
            }
        }
    ]

    StackView.onActivated: {
        quiz.setup(qRoot.config);
        var ttsAvailable = tts.state === TextToSpeech.Ready;
        var quizAvailable = quiz.numQuestionsRemaining >= 0;
        if (ttsAvailable && quizAvailable) {
            var ttsSettingUp = ttsStateChangeConnection.enabled;
            qRoot.state = ttsSettingUp ? "setting-up" : "available";
            qRoot.sayQuestion();
        }
    }
    StackView.onDeactivated: {
        qRoot.state = "setting-up";
    }
    onShowLocaleError: {
        dlgLocaleError.open();
    }

    Quiz {
        id: quiz

        onError: msg => {
                     console.log(msg);
                     qRoot.state = "unavailable";
                 }
        onQuestionChanged: {
            answerInput.text = "";
        }
    }

    // The state of QuizView is kept even if not on top of the stack, so the
    // locale can be updated in the settings and would need to be applied when
    // returning to the view. This would be handled inside the translator.
    SelfUpdatingTranslator {
        id: translator

        onError: msg => {
                     console.log(msg);
                     qRoot.state = "unavailable";
                 }
    }

    // Times Tables:

    Label {
        id: labelNumQuestionsRemaining

        anchors.right: parent.right
        anchors.rightMargin: 2
        anchors.top: parent.top
        anchors.topMargin: 2
        opacity: 0.5
        text: {
            var num = quiz.numQuestionsRemaining;
            return num === 0 ? qsTr("Last question") : num + " " + qsTr("left");
        }
    }

    TextArea {
        id: answerInput

        anchors.bottom: btnReplay.top
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        enabled: false
        focus: false
        font.bold: true

        onFocusChanged: {
            // Force active focus.
            if (!focus)
                return;
            if (inputMethodHints != Qt.ImhDigitsOnly) {
                inputMethodHints = Qt.ImhDigitsOnly;
                // calls forceActiveFocus()
            } else {
                forceActiveFocus();
            }
        }
        onInputMethodHintsChanged: {
            if (focus && inputMethodHints == Qt.ImhDigitsOnly)
                forceActiveFocus();
        }
        onTextChanged: qRoot.check(text)
    }

    Button {
        id: btnReplay

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: qRoot.height / 3
        text: qsTr("Replay")

        onClicked: {
            qRoot.sayQuestion();
            answerInput.focus = true;
        }
    }

    // Text-to-Speech:

    Dialog {
        id: dlgLocaleError

        anchors.centerIn: parent
        margins: 10
        modal: true
        standardButtons: Dialog.Ok
        title: qsTr("Language Error")

        Label {
            elide: Text.ElideRight
            text: qsTr("Language for Text-to-Speech engine not found.")
            width: parent.width
            wrapMode: Text.WordWrap
        }
    }

    ProgressBar {
        id: progressBarTtsLoading

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: answerInput.bottom
        anchors.topMargin: 10
        indeterminate: true
        width: parent.width - 2 * 10
    }

    Label {
        id: labelLocale

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
        anchors.right: parent.right
        anchors.rightMargin: 2
        opacity: 0.5
        text: translator.localeName
    }

    TextToSpeech {
        id: tts

        locale: Qt.locale(translator.localeName)
        rate: QuizSettings.voiceRate

        onLocaleChanged: ttsStateChangeConnection.enabled = true
    }

    Connections {
        id: errorConnection

        function onStateChanged() {
            if (tts.state === TextToSpeech.Error)
                qRoot.state = "unavailable";
        }

        enabled: !ttsStateChangeConnection.enabled
        target: tts
    }

    Connections {
        id: ttsStateChangeConnection

        function onStateChanged() {
            if (tts.state === TextToSpeech.Synthesizing) {
                qRoot.state = "tts-synthesizing";
            } else if (tts.state === TextToSpeech.Speaking) {
                qRoot.state = "available";
                ttsStateChangeConnection.enabled = false;
            } else if (tts.state === TextToSpeech.Error) {
                qRoot.state = "unavailable";
            }
        }

        enabled: false
        target: tts
    }
}
