in SHADOW_OUT
{
    vec4 FragPosLightSpace;
} ShadowIn;

uniform float uShadowPass;

float HandleShadowPass()
{
    if(uShadowPass == 1.0)
    {
        return 1.0;
    }
    else
    {
        return 0.0;
    }
}

float LinearStep(float low, float high, float value)
{
    return clamp((value - low) / (high - low), 0.0, 1.0);
}

float GetHardShadow(sampler2D shadowMap, vec4 fragPosLightSpace, vec3 fragNormal, vec3 fragPos, vec3 lightPos)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // Hard Shadow
    // return currentDepth > closestDepth ? 1.0 :0.0;
    return currentDepth - bias > closestDepth ? 1.0 : 0.0;
}

float GetPcfShadow(sampler2D shadowMap, vec4 fragPosLightSpace, vec3 fragNormal, vec3 fragPos, vec3 lightPos, int taps)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -taps; x <= taps; ++x)
    {
        for(int y = -taps; y <= taps; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    float t = taps;
    // Calculate all taps in x
    t = t * 2 + 1;
    // Multiply with y to get all taps
    t *= t;
    shadow /= t;

    return shadow;
}

float GetVsmShadow(sampler2D shadowMap, vec4 fragPosLightSpace, float minVariance, float lightBleedOffset)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    vec2 moments = texture(shadowMap, projCoords.xy).rg;
    float p = step(projCoords.z, moments.x);
	float variance = max(moments.y - moments.x * moments.x, minVariance);
	
	float d = (projCoords.z - moments.x) * 10;
	float pMax = LinearStep(lightBleedOffset, 1.0, variance / (variance + d * d));
    
	return 1 - min(max(p, pMax), 1.0);
}