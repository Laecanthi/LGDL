#pragma once

#include <LGDL/Types.h>
#include <LGDL/Renderer.h>

namespace LGDL
{
    ShaderProgram CreateShaderProgram(
        const char* vertexPath,
        const char* fragmentPath,
        std::vector<Uniform> uniforms
    );
}
