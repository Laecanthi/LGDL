#version 330 core

in vec4 vColor;
in vec2 vUV;

uniform sampler2D uTexture;

out vec4 FragColor;

vec4 AverageSample(vec2 dir, int amount, vec2 uv, sampler2D tex)
{
    vec4 sum = vec4(0);

    for(int i = 0; i < amount; i++)
    {
        sum += texture(tex, uv + (dir * i));
    }

    sum /= amount;

    return sum;
    //return texture(tex, uv);
}

void main()
{
    vec4 texColor = texture(uTexture, vUV);
    vec4 blurColor = AverageSample(vec2(0.0025,0.005), 5, vUV, uTexture);
    blurColor += AverageSample(vec2(-0.0025,0.005), 5, vUV, uTexture);
    blurColor /= 2;

    float mask = 1 - texColor.w;

    //texColor = max(texColor,blurColor);
    blurColor -= texColor;
    
    texColor = vec4(vColor.xyz, texColor.w);
    texColor = mix(texColor, blurColor, mask);

    //FragColor = vec4(vColor.xyz, texColor.w);
    FragColor = texColor;
    //FragColor = vec4(vec3(mask),1);

    //FragColor = vec4(vUV.xy, 0, 1);
}