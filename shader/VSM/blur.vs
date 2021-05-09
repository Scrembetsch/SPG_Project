#version 450

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 vTexCoord;

void main(void)
{
    gl_Position = vec4(aPosition, 0.0, 1.0);
    vTexCoord = aTexCoords;
}