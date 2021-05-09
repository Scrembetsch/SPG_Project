#version 430

INCLUDE "shader/include/ShadowPass.fsh"
INCLUDE "shader/include/Mapping/Parallax.fsh"

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
} FsIn;

layout (binding = 0) uniform sampler2D uDiffuseMap;
layout (binding = 1) uniform sampler2D uNormalMap;
layout (binding = 2) uniform sampler2D uDepthMap;
layout (binding = 3) uniform sampler2D uShadowMap;
uniform vec4 uColor;

uniform float uHeightScale;
uniform float uSteps;
uniform float uRefinementSteps;

out vec4 oColor;

void main()
{           
    vec3 viewDir = normalize(ParallaxIn.TangentViewPos - ParallaxIn.TangentFragPos);
    
    vec2 texCoords = ParallaxMapping(uDepthMap, FsIn.TexCoords, viewDir, uHeightScale, uSteps, uRefinementSteps);       
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
    {
        discard;
    }

    vec3 normal = texture(uNormalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
   
    vec3 color = texture(uDiffuseMap, texCoords).rgb;
    vec3 ambient = 0.1 * color;

    vec3 lightDir = normalize(ParallaxIn.TangentLightPos - ParallaxIn.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;

    float shadow = GetVsmShadow(uShadowMap, ShadowIn.FragPosLightSpace, 0.2, 0.5);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    oColor = vec4(lighting, 1.0) * uColor;
}