#version 430

layout (binding = 0) uniform sampler3D uTex;
uniform int uWidth;
uniform int uHeight;
uniform int uDepth;
// uniform mat4 uProjection;
// uniform mat4 uView;
// uniform mat4 uModel;
out vec3 vTexPos;
out int vIndex;

void main(void) { 
    int id = gl_VertexID;
    int x = id % uWidth;
    int y = (id / uWidth) % uHeight;
    int z = (id / (uWidth * uHeight)) % uDepth;

    float widthStep = 1.0f / (uWidth - 1);
    float heightStep = 1.0f / (uHeight - 1);
    float depthStep = 1.0f / (uDepth - 1);

    vec3 xyz = vec3(x, y, z);
    // gl_Position = uProjection * uView * uModel * vec4(xyz, 1.0);
    gl_Position = vec4(xyz, 1.0);
    vTexPos = vec3(x * widthStep, y * heightStep, z * depthStep);
    
    int b1 = int(texture(uTex, vTexPos                                          ) .r < 0.8f);
    int b2 = int(texture(uTex, vTexPos + vec3(widthStep , 0.0       , 0.0       )).r < 0.8f);
    int b3 = int(texture(uTex, vTexPos + vec3(widthStep , 0.0       , depthStep )).r < 0.8f);
    int b4 = int(texture(uTex, vTexPos + vec3(0.0       , 0.0       , depthStep )).r < 0.8f);
    int b5 = int(texture(uTex, vTexPos + vec3(0.0       , heightStep, 0.0       )).r < 0.8f);
    int b6 = int(texture(uTex, vTexPos + vec3(widthStep , heightStep, 0.0       )).r < 0.8f);
    int b7 = int(texture(uTex, vTexPos + vec3(widthStep , heightStep, depthStep )).r < 0.8f);
    int b8 = int(texture(uTex, vTexPos + vec3(0.0       , heightStep, depthStep )).r < 0.8f);
    
    vIndex = (b1 << 7) | (b2 << 6) | (b3 << 5) | (b4 << 4) | 
                (b5 << 3) | (b6 << 2) | (b7 << 1) | b8;
}
