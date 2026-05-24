qt_add_executable(OTTQ main.cpp)

target_link_libraries(OTTQ PRIVATE
    Qt6::Quick
    Qt6::TextToSpeech
    Qt6::StateMachine
    TimesTables
    Tts
)

target_include_directories(OTTQ PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

set_source_files_properties(QuizSettings.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(OTTQ
    URI OttqApp
    VERSION 0.1
    QML_FILES
        Main.qml
        QuizConfigView.qml
        QuizView.qml
        SettingsView.qml
        QuizSettings.qml
    SOURCES
        quiz_backend.hpp
        quiz_backend.cpp
        settings_backend.hpp
        settings_backend.cpp
        language_name.hpp
        factor_range.hpp
        factor_range.cpp
        quiz_state_machine.hpp
        quiz_state_machine.cpp
        quiz_config.hpp
        quiz_config.cpp
        quiz.hpp
        quiz.cpp
        question.hpp
        question.cpp
        self_updating_translator.hpp
        self_updating_translator.cpp
        language_list_model.hpp
        language_list_model.cpp
        auto_locale.hpp
        auto_locale.cpp
)

qt_add_translations(OTTQ
     TS_FILE_BASE qml
     TS_FILE_DIR i18n
     RESOURCE_PREFIX qt/qml/OttqApp/i18n
)
