#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;

// camera transform
uniform mat3 uView;

// out
out vec4 vColor;

void main()
{
    vec3 transformed = uView * vec3(aPos, 1.0);

    gl_Position = vec4(transformed.xy, 0.0, 1.0);

    vColor = aColor;
}