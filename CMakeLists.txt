cmake_minimum_required(VERSION 3.16)

project(OpenGLTest)

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
    src/utils/FileIO.cpp
    src/utils/Math.cpp
)

target_include_directories(LGDL PUBLIC
    include
)

target_include_directories(LGDL PRIVATE
    ${glad_SOURCE_DIR}/include
)

target_link_libraries(LGDL PRIVATE glad)