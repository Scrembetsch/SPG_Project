#version 430

layout (binding = 0) uniform sampler2D uSampler;

smooth in vec2 vTexCoord;
flat in vec4 vColorPart;

out vec4 oColor;

void main()
{
  vec4 vTexColor = texture2D(uSampler, vTexCoord);
  oColor = vec4(vTexColor.xyz, 1.0) * vColorPart;
}