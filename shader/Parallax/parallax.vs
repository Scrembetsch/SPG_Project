#version 430

INCLUDE "shader/include/ShadowPass.vsh"
INCLUDE "shader/include/Mapping/Parallax.vsh"

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
} VsOut;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

uniform vec3 uLightPos;
uniform vec3 uViewPos;

void main()
{
    if(HandleShadowPass(uModel, aPos) == 1.0)
    {
        return;
    }

    VsOut.FragPos = vec3(uModel * vec4(aPos, 1.0));   
    VsOut.TexCoords = aTexCoords;   
    
    CalculateTangentSpace(mat3(uModel), aTangent, aBitangent, aNormal, uLightPos, uViewPos, VsOut.FragPos);

    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}
