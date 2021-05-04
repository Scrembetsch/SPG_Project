#version 430

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

layout (binding = 0) uniform sampler2D uDiffuseMap;
layout (binding = 1) uniform sampler2D uNormalMap;
layout (binding = 2) uniform sampler2D uDepthMap;

uniform float uHeightScale;
uniform float uSteps;
uniform float uRefinmentSteps;

out vec4 oColor;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    float layerDepth = 1.0 / uSteps;
    float currentLayerDepth = 0.0;
    vec2 P = viewDir.xy / viewDir.z * uHeightScale; 
    vec2 deltaTexCoords = P / uSteps;
  
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(uDepthMap, currentTexCoords).r;

    // Steps
    while(currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(uDepthMap, currentTexCoords).r;  
        currentLayerDepth += layerDepth;  
    }

    layerDepth /= uRefinmentSteps;
    deltaTexCoords /= uRefinmentSteps;

    // Refinment steps
    while(currentLayerDepth > currentDepthMapValue)
    {
        currentTexCoords += deltaTexCoords;
        currentDepthMapValue = texture(uDepthMap, currentTexCoords).r;  
        currentLayerDepth -= layerDepth;  
    }
    
    return currentTexCoords;
}

void main()
{           
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = fs_in.TexCoords;
    
    texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);       
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
    {
        discard;
    }

    vec3 normal = texture(uNormalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
   
    vec3 color = texture(uDiffuseMap, texCoords).rgb;
    vec3 ambient = 0.1 * color;

    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    oColor = vec4(ambient + diffuse + specular, 1.0);
}