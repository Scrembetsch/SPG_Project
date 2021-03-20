#version 430

layout (location = 0) in vec2 aPosition;

uniform float uLayer;
uniform float uHeight;

out vec3 vPosition;

void main(void)
{ 
    gl_Position = vec4(aPosition, 0.0, 1.0);
    vPosition = vec3(aPosition.x, aPosition.y + uHeight, uLayer);
}
