#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor; // doesn't actually do anything
layout (location = 2) in vec2 aUV;

// instance data
layout (location = 3) in vec2 iPosition;
layout (location = 4) in vec2 iScale;
layout (location = 5) in float iRotation;
layout (location = 6) in vec4 iColor;
layout (location = 7) in vec2 iMinUV;
layout (location = 8) in vec2 iMaxUV;


// camera transform

uniform mat3 uView;

// out

out vec4 vColor;
out vec2 vUV;

void main()
{
    vec2 localPos = aPos;

    localPos *= iScale;

    float s = sin(iRotation);
    float c = cos(iRotation);

    vec2 worldPos = {localPos.x * c - localPos.y * s, localPos.x * s + localPos.y * c};

    worldPos += iPosition;

    vec3 transformed = uView * vec3(worldPos, 1.0);
    vec2 screenPos = transformed.xy;

    gl_Position = vec4(screenPos, 0.0, 1.0);

    //gl_Position = vec4(aPos, 0.0, 1.0);

    vColor = iColor;

    vec2 UVScale = iMaxUV - iMinUV;

    vUV = aUV * UVScale + iMinUV;
}