#version 430

layout (binding = 0) uniform sampler3D uTex;

in vec3 vTexPosGeo;
in vec3 vTexPos;

out vec4 oColor;

void main(void) {
    float f = texture(uTex, vTexPosGeo).r;
    // float f = texture(uTex, vTexPos).r;
    // f *= -1;
    oColor = vec4(cos(vTexPosGeo) * f, 1.0);
    // oColor = vec4(f, f, f, 1.0f);
}
