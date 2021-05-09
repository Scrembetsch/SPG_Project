out PARALLAX_OUT
{
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} ParallaxOut;

void CalculateTangentSpace(mat3 model, vec3 tangent, vec3 bitangent, vec3 normal, vec3 lightPos, vec3 viewPos, vec3 fragPos)
{
    vec3 T = normalize(model * tangent);
    vec3 B = normalize(model * bitangent);
    vec3 N = normalize(model * normal);
    mat3 TBN = transpose(mat3(T, B, N));

    ParallaxOut.TangentLightPos = TBN * lightPos;
    ParallaxOut.TangentViewPos  = TBN * viewPos;
    ParallaxOut.TangentFragPos  = TBN * fragPos;
}