#version 430

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
} fs_in;

layout (binding = 0) uniform sampler2D uDiffuseMap;

out vec4 oColor;

void main()
{           
    oColor = texture(uDiffuseMap, fs_in.TexCoords);
}