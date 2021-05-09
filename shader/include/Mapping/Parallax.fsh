in PARALLAX_OUT
{
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} ParallaxIn;

vec2 ParallaxMapping(sampler2D depthMap, vec2 originalTexCoords, vec3 viewDir, float scale, float steps, float refineSteps)
{
    float layerDepth = 1.0 / steps;
    float currentLayerDepth = 0.0;
    vec2 P = viewDir.xy / viewDir.z * scale; 
    vec2 deltaTexCoords = P / steps;
  
    vec2  currentTexCoords     = originalTexCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;

    // Steps
    while(currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
        currentLayerDepth += layerDepth;  
    }

    layerDepth /= refineSteps;
    deltaTexCoords /= refineSteps;

    // Refinment steps
    while(currentLayerDepth > currentDepthMapValue)
    {
        currentTexCoords += deltaTexCoords;
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
        currentLayerDepth -= layerDepth;  
    }
    
    return currentTexCoords;
}