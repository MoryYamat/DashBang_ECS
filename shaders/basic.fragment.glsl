#version 330 core

uniform vec3 uBaseColor;

out vec4 FragColor;

void main()
{
	FragColor = vec4(uBaseColor, 1.0);
}