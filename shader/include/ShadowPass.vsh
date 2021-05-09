out SHADOW_OUT
{
    vec4 FragPosLightSpace;
} ShadowOut;

uniform mat4 uLightSpaceMatrix;
uniform float uShadowPass;

float HandleShadowPass(mat4 model, vec3 pos)
{
    if(uShadowPass == 1.0)
    {
        gl_Position = uLightSpaceMatrix * model * vec4(pos, 1.0);
        return 1.0;
    }
    else
    {
        ShadowOut.FragPosLightSpace = uLightSpaceMatrix * model * vec4(pos, 1.0);
        return 0.0;
    }
}