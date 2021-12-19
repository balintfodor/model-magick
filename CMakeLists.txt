cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(model-magick
    VERSION 1.0
    LANGUAGES CXX
)

enable_testing()

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
        FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS KEEP_RPATHS)

add_subdirectory(src/model-magick)
add_subdirectory(apps)
add_subdirectory(tests)
