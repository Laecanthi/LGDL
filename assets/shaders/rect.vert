#version 330 core

layout (location = 0) in vec2 aPos;

uniform vec2 uPosition;
uniform vec2 uSize;

void main()
{
    vec2 worldPos = aPos;

    worldPos *= uSize;
    worldPos += uPosition;

    gl_Position = vec4(worldPos, 0.0, 1.0);
}