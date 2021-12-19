cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(LIB_TARGET_NAME model-magick)

add_library(${LIB_TARGET_NAME}
    include/model-magick/mesh.h
    include/model-magick/importer.h
    importer.cpp
)

set_target_properties(${LIB_TARGET_NAME}
    PROPERTIES CXX_STANDARD 17
)

if(MSVC)
    target_compile_definitions(${LIB_TARGET_NAME}
        PRIVATE
            # assimp can cause compilation error if this is not set
            -DNOMINMAX
    )
    target_compile_options(${LIB_TARGET_NAME}
        PRIVATE
            /W4
            /permissive-
    )
else()
    target_compile_options(${LIB_TARGET_NAME}
        PRIVATE
            -Wall
            -Wextra
            -Werror
            -Wpedantic
        )
endif()

target_link_libraries(${LIB_TARGET_NAME}
    PUBLIC
        CONAN_PKG::eigen
        CONAN_PKG::onetbb
    PRIVATE
        CONAN_PKG::libigl
        CONAN_PKG::assimp
)

target_include_directories(${LIB_TARGET_NAME}
    PUBLIC include
)
