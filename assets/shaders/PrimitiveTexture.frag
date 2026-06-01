#version 330 core

in vec4 vColor;
in vec2 vUV;

uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(uTexture, vUV);

    FragColor = vec4(vColor.xyz, texColor.w);

    //FragColor = vec4(vUV.xy, 0, 1);
}