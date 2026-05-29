#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;

// screen dimensions
uniform vec2 uScreen; // input should be the width and height of screen in pixels

// out
out vec4 vColor;
out vec2 vUV;

void main()
{
    vec2 screenPos = aPos / (uScreen / 2); // turns UV coords into pixels

    screenPos -= vec2(1); // shifts center to be in the bottom left corner rather than the center

    gl_Position = vec4(screenPos, 0.0, 1.0);

    vColor = aColor;
    vUV = aUV;
}