cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(APP_TARGET_NAME model-magick-cli)

add_executable(${APP_TARGET_NAME}
    main.cpp
)

set_target_properties(${APP_TARGET_NAME}
    PROPERTIES CXX_STANDARD 17
)
target_compile_options(${APP_TARGET_NAME}
    PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->"
)

target_link_libraries(${APP_TARGET_NAME}
    PUBLIC
        CONAN_PKG::assimp
    PRIVATE
        model-magick
)

# smoke test
add_test(
    NAME
        ${APP_TARGET_NAME}
    COMMAND
        ${APP_TARGET_NAME}
    WORKING_DIRECTORY
        $<TARGET_FILE_DIR:${APP_TARGET_NAME}>
)
