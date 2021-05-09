#version 450

INCLUDE "shader/include/ShadowPass.vsh"

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT
{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
} VsOut;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
    if(HandleShadowPass(uModel, aPos) == 1.0)
    {
        return;
    }

    VsOut.FragPos = vec3(uModel * vec4(aPos, 1.0));   
    VsOut.Normal = transpose(inverse(mat3(uModel))) * aNormal;
    VsOut.TexCoords = aTexCoords;

    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
