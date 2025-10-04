#version 330 core

in vec2 vUV;
in vec3 vNormal;

out vec4 FragColor;

uniform bool uHasBaseColorTex;
uniform sampler2D uBaseColorTex;
uniform vec3 uBaseColor;

void main()
{
	vec3 base = uHasBaseColorTex ? texture(uBaseColorTex, vUV).rgb
									: uBaseColor;
	vec3 n = normalize(vNormal);
	float ndl = clamp(dot(n,normalize(vec3(0,1,0))), 0.2, 1.0);

	FragColor = vec4(base * ndl, 1.0);
}