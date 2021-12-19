cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(TEST_TARGET_NAME "test-model-magick")

add_executable(${TEST_TARGET_NAME}
    test_main.cpp
    test_importer.cpp
)

set_target_properties(${TEST_TARGET_NAME}
    PROPERTIES CXX_STANDARD 17
)

if(MSVC)
    target_compile_options(${TEST_TARGET_NAME}
        PRIVATE
            /W4
            /permissive-
    )
else()
    target_compile_options(${TEST_TARGET_NAME}
        PRIVATE
            -Wall
            -Wextra
            -Werror
            -Wpedantic
        )
endif()

target_link_libraries(${TEST_TARGET_NAME}
    PRIVATE
        CONAN_PKG::catch2
        model-magick
)

add_test(
    NAME
        ${TEST_TARGET_NAME}
    COMMAND
        ${TEST_TARGET_NAME}
    WORKING_DIRECTORY
        $<TARGET_FILE_DIR:${TEST_TARGET_NAME}>
)

add_custom_target(copy-test-assets
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets $<TARGET_FILE_DIR:${TEST_TARGET_NAME}>/assets
)

add_dependencies(${TEST_TARGET_NAME} copy-test-assets)
