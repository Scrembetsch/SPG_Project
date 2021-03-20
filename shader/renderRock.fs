#version 430

layout (binding = 0) uniform sampler3D uTex;

in vec3 vTexPosGeo;

out vec4 oColor;

void main(void)
{
    float f = texture(uTex, vTexPosGeo).r;
    oColor = vec4(vTexPosGeo, 1.0);
}
