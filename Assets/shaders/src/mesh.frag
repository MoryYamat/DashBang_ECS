#version 450 core

uniform vec4 uBaseColorFactor;

uniform sampler2D uBaseColorTexture;
uniform bool uHasBaseColorTexture;

uniform vec3 uLightDirWorld;
uniform vec3 uLightColor;
uniform float uAmbientStrength;

in vec2 vTexCoord0;
in vec3 vNormalWorld;

out vec4 FragColor;

void main()
{
    vec4 baseColor = uBaseColorFactor;

    if (uHasBaseColorTexture)
    {
        baseColor *= texture(uBaseColorTexture, vTexCoord0);
    }

    vec3 N = normalize(vNormalWorld);
    vec3 L = normalize(-uLightDirWorld);

    float ndotl = max(dot(N, L), 0.0);

    vec3 diffuse = uLightColor * ndotl;
    vec3 ambient = vec3(uAmbientStrength);

    vec3 color = baseColor.rgb * (ambient + diffuse);

    FragColor = vec4(color, baseColor.a);
}