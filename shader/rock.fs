#version 430

in vec3 vPosition;

out float oColor;

vec2 pillars[3]= vec2[](
    vec2(0.5f,0.5f),
    vec2(-0.7f,0.1f),
    vec2(0.50f,-0.30f)
);

void main(void)
{
    float f = 0;
    for(int i = 0; i < 1; i++){
        f += 1 / length(vPosition.yz - pillars[i]) - 1;
    }
    oColor = f;
    // float sinX = sin(vPosition.x * 5.8905);
    // float cosY = cos(vPosition.y * 5.8905);
    // float cosZ = cos(vPosition.z * 5.8905);
    // oColor = (sinX * sinX + cosY * cosY + cosZ * cosZ) * (1.0f / 3.0f);
    // oColor = vec4(Color, Color, Color, 1.0f);
}
