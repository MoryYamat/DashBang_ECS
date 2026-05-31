#version 450 core

in vec2 vUv;
in vec4 vColor;

uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
    float alpha = texture(uTexture, vUv).r;
    FragColor = vec4(vColor.rgb, vColor.a * alpha);
}