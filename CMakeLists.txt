cmake_minimum_required(VERSION 3.16)

project(LGDL)

set(CMAKE_CXX_STANDARD 17)

# -------------------------
# GLAD
# -------------------------

add_library(glad
    thirdparty/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    thirdparty/glad/include
)
# -------------------------
# LGDL Library
# -------------------------
add_library(LGDL
    src/core/LGDL.cpp
    src/renderer/RendererCore.cpp
    src/renderer/RendererDraw.cpp
    src/graphics/Mesh.cpp
    src/graphics/Shader.cpp
    src/graphics/Primitive.cpp
    src/graphics/Glyphs.cpp
    src/utils/FileIO.cpp
    src/utils/Math.cpp
    src/graphics/Texture.cpp
    thirdparty/stb/stb_image.cpp
)

target_include_directories(LGDL PUBLIC
    include
)

target_include_directories(LGDL PRIVATE
    ${glad_SOURCE_DIR}/include
)

target_include_directories(LGDL PUBLIC
    include
    thirdparty
)

target_link_libraries(LGDL PRIVATE glad)