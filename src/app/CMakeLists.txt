qt_add_executable(TimesTablesQuiz main.cpp)

target_link_libraries(TimesTablesQuiz PRIVATE
    Qt6::Quick
    Qt6::TextToSpeech
    Qt6::StateMachine
    TimesTables
    Tts
)

target_include_directories(TimesTablesQuiz PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

set_source_files_properties(QuizSettings.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(TimesTablesQuiz
    URI TtqApp
    VERSION 0.1
    QML_FILES
        Main.qml
        QuizConfigView.qml
        QuizView.qml
        SettingsView.qml
        QuizSettings.qml
    SOURCES
        language_name.hpp
        factor_range.hpp
        factor_range.cpp
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

qt_add_translations(TimesTablesQuiz
     TS_FILE_BASE qml
     TS_FILE_DIR i18n
     RESOURCE_PREFIX qt/qml/TtqApp/i18n
)
