#version 450

// in VS_OUT {
//     vec3 FragPos;
//     vec2 TexCoords;
//     vec3 Normal;
// } FsIn;

layout (binding = 0) uniform sampler2D uDiffuseMap;
layout (binding = 1) uniform sampler2D uShadowMap;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform vec4 uColor;

out vec4 oColor;

void main()
{

    oColor = vec4(1.0);
    return;
    // if(HandleShadowPass() == 1.0)
    // {
    //     return;
    // }

    // vec3 color = texture(uDiffuseMap, FsIn.TexCoords).rgb;
    // vec3 normal = normalize(FsIn.Normal);
    // vec3 lightColor = vec3(0.3);
    // // ambient
    // vec3 ambient = 0.3 * color;
    // // diffuse
    // vec3 lightDir = normalize(uLightPos - FsIn.FragPos);
    // float diff = max(dot(lightDir, normal), 0.0);
    // vec3 diffuse = diff * lightColor;
    // // specular
    // vec3 viewDir = normalize(uViewPos - FsIn.FragPos);
    // vec3 reflectDir = reflect(-lightDir, normal);
    // float spec = 0.0;
    // vec3 halfwayDir = normalize(lightDir + viewDir);
    // spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    // vec3 specular = spec * lightColor;
    // // calculate shadow
    // // float shadow = GetHardShadow(uShadowMap, ShadowIn.FragPosLightSpace, FsIn.Normal, FsIn.FragPos, uLightPos);
    // // float shadow = GetVsmShadow(uShadowMap, ShadowIn.FragPosLightSpace, 0.2, 0.5);
    // vec3 lighting = (ambient + (diffuse + specular)) * color;

    // oColor = vec4(lighting, 1.0) * uColor;
    // oColor = texture(uShadowMap, FsIn.TexCoords);
}