#version 430

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;

    vec4 FragPosLightSpace;
} fs_in;

layout (binding = 0) uniform sampler2D uDiffuseMap;
layout (binding = 1) uniform sampler2D uShadowMap;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform vec4 uColor;
uniform float uShadowPass;

out vec4 oColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(uShadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(uLightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    float shadow = 0.0;
    // Hard Shadow
    // shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
    // shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    
    // PCF
    // shadow = 0.0;
    // int taps = 3;
    // vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);
    // for(int x = -taps; x <= taps; ++x)
    // {
    //     for(int y = -taps; y <= taps; ++y)
    //     {
    //         float pcfDepth = texture(uShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
    //         shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
    //     }    
    // }
    // float t = taps;
    // t = t*2 + 1;
    // t *= t;
    // shadow /= t;
    
    // ESM
    // shadow = 0.0;
    // shadow = clamp(exp(30 * (closestDepth - currentDepth)), 0.0, 1.0);
}

void main()
{           
    if(uShadowPass == 1.0)
    {
        return;
    }
    vec3 color = texture(uDiffuseMap, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDir = normalize(uLightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(uViewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    oColor = vec4(lighting, 1.0) * uColor;
    // oColor = texture(uShadowMap, fs_in.TexCoords);
}