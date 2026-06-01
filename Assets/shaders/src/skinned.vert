#version 450 core

const int MAX_BONES = 128;

layout(location = 0) in vec3 aPos;
layout(location = 4) in uvec4 aJoints;
layout(location = 5) in vec4 aWeights;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

uniform mat4 uSkinMatrices[MAX_BONES];

void main()
{
    mat4 skin = 
        aWeights.x * uSkinMatrices[aJoints.x] +
        aWeights.y * uSkinMatrices[aJoints.y] +
        aWeights.z * uSkinMatrices[aJoints.z] +
        aWeights.w * uSkinMatrices[aJoints.w];

    vec4 skinnedPos = skin * vec4(aPos, 1.0);

    gl_Position = uProj * uView * uModel * skinnedPos;
}