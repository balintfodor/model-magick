cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(LIB_TARGET_NAME model-magick)

add_library(${LIB_TARGET_NAME}
    include/model-magick/ConstProvider.h
    include/model-magick/Consumer.h
    include/model-magick/GeneralizedWindingNumberCalculator.h
    include/model-magick/Image.h
    include/model-magick/ImageWriter.h
    include/model-magick/Mesh.h
    include/model-magick/MeshImporter.h
    include/model-magick/MeshStatsCalculator.h
    GeneralizedWindingNumberCalculator.cpp
    ImageWriter.cpp
    MeshImporter.cpp
    MeshStatsCalculator.cpp
)

set_target_properties(${LIB_TARGET_NAME}
    PROPERTIES CXX_STANDARD 17
)

if(MSVC)
    target_compile_definitions(${LIB_TARGET_NAME}
        PUBLIC
            -D_USE_MATH_DEFINES
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
        CONAN_PKG::lodepng
        CONAN_PKG::assimp
)

target_include_directories(${LIB_TARGET_NAME}
    PUBLIC include
)
