#version 430

// layout (binding = 0) uniform sampler3D uTex;

// in vec3 vTexPos;

out vec4 oColor;

void main(void) {
    // float f = texture(uTex, vTexPos).r;
    oColor = vec4(1.0, 1.0, 1.0, 1.0);
}
