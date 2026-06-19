#version 450 core

uniform vec4 uBaseColorFactor;

uniform sampler2D uBaseColorTexture;
uniform bool uHasBaseColorTexture;

in vec2 vTexCoord0;

out vec4 FragColor;

void main()
{
    vec4 baseColor = uBaseColorFactor;

    if(uHasBaseColorTexture)
    {
        baseColor *= texture(uBaseColorTexture, vTexCoord0);
    }

    FragColor = baseColor;
}