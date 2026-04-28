#version 450 core

layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    vec2 positions[3] = vec2[](
        vec2(-0.8, -0.8),
        vec2( 0.8, -0.8),
        vec2( 0.0,  0.8)
    );

    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);

}