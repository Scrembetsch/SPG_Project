#version 430

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    // Default
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;

    // Shadow Mapping
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

uniform mat4 uLightSpaceMatrix;
uniform float uShadowPass;

void main()
{
    if(uShadowPass == 1.0)
    {
        gl_Position = uLightSpaceMatrix * uModel * vec4(aPos, 1.0);
        return;
    }
    
    vs_out.FragPos = vec3(uModel * vec4(aPos, 1.0));   
    vs_out.Normal = transpose(inverse(mat3(uModel))) * aNormal;
    vs_out.TexCoords = aTexCoords;

    vs_out.FragPosLightSpace = uLightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
