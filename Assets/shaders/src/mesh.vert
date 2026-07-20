#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord0;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 vTexCoord0;
out vec3 vNormalWorld;

void main()
{
    vec4 worldPos = uModel * vec4(aPos, 1.0);

    vNormalWorld = normalize(mat3(uModel) * aNormal);
    vTexCoord0 = aTexCoord0;

    gl_Position = uProj * uView * worldPos;
}