#version 450

in vec2 vTexCoord;

layout (location = 0) uniform sampler2D uFilterMap;
uniform float uBlurAmount;
out vec4 oColor;

void main(void)
{
    vec2 sum = vec2(0.0, 0.0);

    vec2 texelSize = 1.0 / vec2(textureSize(uFilterMap, 0));
    for(float x = -uBlurAmount; x <= uBlurAmount; x++) {
        for(float y = -uBlurAmount; y <= uBlurAmount; y++) {
            vec2 offset = vec2(x, y) * texelSize;
            
            float depth = texture(uFilterMap, vTexCoord + offset).r;

            float dx = dFdx(depth);
            float dy = dFdy(depth);
            float e2 = depth * depth + 0.25 * (dx * dx + dy * dy);

            sum += vec2(depth, e2);
        }
    }
    float numSamples = uBlurAmount;
    numSamples = numSamples * 2 + 1;
    numSamples *= numSamples;
    sum /= numSamples;

    oColor = vec4(sum, 0.0, 1.0);
}