#version 430

in vec3 vPosition;

out float oColor;

vec2 cPillars[3]= vec2[](
    vec2(0.5f,  -0.3f),
    vec2(-0.4f, 0.0f),
    vec2(0.4f, 0.6f)
);

float CreatePillars(vec2 position, int pillars, float pillarSize)
{
    float f = 0.0f;
    for(int i = 0; i < pillars; i++){
        f += 1 / (length(position - cPillars[i]) * 1.5) - 1;
    }
    return f;
}

vec2 RotatePosition(vec2 position, float angle)
{
    mat2 rot;
    rot[0] = vec2(cos(angle), -sin(angle));
    rot[1] = vec2(sin(angle), cos(angle));
    return rot * position;
}

void main(void) {
    vec2 rotPos1 = RotatePosition(vPosition.xz, vPosition.y);
    vec2 rotPos2 = RotatePosition(vPosition.xz, -vPosition.y * 5);

    float f = 0.0f;
    // Create pillars
    f += CreatePillars(rotPos1, 3, 1.5);
    f += CreatePillars(rotPos2, 1, 5);

    // Flow channel
    f -= 1 / length(rotPos1) - 1;

    // Shelve
    float c = abs(cos(vPosition.y));
    f += pow(c, 1.5);

    // Remove outer
    float len = length(rotPos1);
    f = f - pow(len, 3);

    oColor = f;
}
