#version 430

layout (binding = 0) uniform sampler3D uTex;
uniform float uStep;
out vec3 vTexPos;
out int vIndex;

void main(void) { 
    int id = gl_VertexID;
    int x = id & 0x3F;
    int y = (id >> 6) & 0x3F;
    int z = (id >> 12) & 0x3F;

    vec3 xyz = vec3(x, y, z);
    gl_Position = vec4(xyz, 1.0);
    vTexPos = xyz * uStep;
    
    int b1 = int(texture(uTex, vTexPos).r < 0.5f);
    int b2 = int(texture(uTex, vTexPos + vec3(uStep, 0.0, 0.0)).r < 0.5f);
    int b3 = int(texture(uTex, vTexPos + vec3(uStep, 0.0, uStep)).r < 0.5f);
    int b4 = int(texture(uTex, vTexPos + vec3(0.0, 0.0, uStep)).r < 0.5f);
    int b5 = int(texture(uTex, vTexPos + vec3(0.0, uStep, 0.0)).r < 0.5f);
    int b6 = int(texture(uTex, vTexPos + vec3(uStep, uStep, 0.0)).r < 0.5f);
    int b7 = int(texture(uTex, vTexPos + vec3(uStep, uStep, uStep)).r < 0.5f);
    int b8 = int(texture(uTex, vTexPos + vec3(0.0, uStep, uStep)).r < 0.5f);
    vIndex = (b1 << 7) | (b2 << 6) | (b3 << 5) | (b4 << 4) | 
                (b5 << 3) | (b6 << 2) | (b7 << 1) | b8;
}
