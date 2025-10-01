#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform bool uHasBaseColorTex;
uniform sampler2D uBaseColorTex;
uniform vec3 uBaseColor;

void main()
{
	vec3 albedo = uHasBaseColorTex ? texture(uBaseColorTex, vUV).rgb
									: uBaseColor;

	FragColor = vec4(albedo, 1.0);
}