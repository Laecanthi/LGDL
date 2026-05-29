#version 330 core

in vec4 vColor;
in vec2 vUV;

out vec4 FragColor;

void main()
{
    FragColor = vColor;
    //FragColor = vec4(vUV, 0, 1);
}