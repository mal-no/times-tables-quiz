cmake_minimum_required(VERSION 3.28)

project(TimesTablesQuiz VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Needed by clang-tidy

set(TranslatorImplementations QT_TRANSLATOR BOOST_TRANSLATOR)
# Use capital letters for the names because they are used as cpp text macros
# and should match their naming convention.
set(Translator QT_TRANSLATOR)
#set(Translator BOOST_TRANSLATOR)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Android"
   AND ${Translator} MATCHES BOOST_TRANSLATOR)
    set(Translator QT_TRANSLATOR)
    # Boost.locale will not work for cross compiling.
endif()

if(NOT ${Translator} IN_LIST TranslatorImplementations)
    message(FATAL_ERROR
            "-- Could NOT find translator implementation ${Translator} "
            "(Should be one of ${TranslatorImplementations})")
endif()

find_package(Qt6
    REQUIRED COMPONENTS
        Quick
        Core
        LinguistTools
        TextToSpeech
        Test
        StateMachine
)
if(${Translator} MATCHES BOOST_TRANSLATOR)
    find_package(Boost
        # Find boost via config file not FindBoost module.
        # Requires minimum Boost 1.70.
        CONFIG
        REQUIRED COMPONENTS
            locale
    )
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Set up translator:
add_compile_definitions(${Translator})
if(${Translator} MATCHES QT_TRANSLATOR)
    set(TranslationDomain "Translator")
    set(TranslationFileEnding "qm")
elseif(${Translator} MATCHES BOOST_TRANSLATOR)
    set(TranslationDir "src/timestables/i18n")
    set(TranslationFileEnding "mo")
endif()
add_compile_definitions(TRANSLATION_DOMAIN="${TranslationDomain}")
add_compile_definitions(TRANSLATION_DIR="${TranslationDir}")
add_compile_definitions(TRANSLATION_FILE_ENDING="${TranslationFileEnding}")

qt_standard_project_setup(
    REQUIRES 6.8
    SUPPORTS_UP_TO 6.11
    I18N_TRANSLATED_LANGUAGES de
)

add_subdirectory(src/app)
add_subdirectory(src/timestables)
add_subdirectory(src/tts)
if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    # don't cross compile the unit tests for now
    add_subdirectory(test)
endif()
