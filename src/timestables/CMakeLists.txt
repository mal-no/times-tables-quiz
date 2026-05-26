add_library(TimesTables STATIC
)

target_link_libraries(TimesTables PRIVATE
    Qt6::Core
)

if(${Translator} MATCHES BOOST_TRANSLATOR)
    add_custom_target(CopyGettextFiles
        ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_gettext_files.cmake
    )
    add_dependencies(TimesTables CopyGettextFiles)

    target_link_libraries(TimesTables PRIVATE
        Boost::locale
    )
endif()

target_include_directories(TimesTables PRIVATE
    "${PROJECT_SOURCE_DIR}/include/timestables"
)
