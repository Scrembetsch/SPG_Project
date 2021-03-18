#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    // Default
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
    //Normal Mapping
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    // Shadow Mapping
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;
uniform mat4 uLightSpaceMatrix;

uniform vec3 uLightPos;
uniform vec3 uViewPos;

void main()
{
    vs_out.FragPos = aPos;
    vs_out.Normal = transpose(inverse(mat3(uModel))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    
    // Calculate tangent and bitangent in vertex shader -> Easier but not 100% accurate
    vec3 tangent;
    vec3 c1 = cross(aNormal.xyz, vec3(0.0, 0.0, 1.0));
    vec3 c2 = cross(aNormal.xyz, vec3(0.0, 1.0, 0.0));
    if(length(c1) > length(c2))
    {
        tangent = normalize(c1);
    }
    else
    {
        tangent = normalize(c2);
    }

    mat3 normalMatrix = transpose(inverse(mat3(uModel)));
    vec3 T = normalize(normalMatrix * tangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    vs_out.TangentLightPos = TBN * uLightPos;
    vs_out.TangentViewPos  = TBN * uViewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
    
    vs_out.FragPosLightSpace = uLightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}