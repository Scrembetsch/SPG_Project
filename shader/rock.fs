#version 430

in vec3 vPosition;

out float oColor;

void main(void) {
    float sinX = sin(vPosition.x * 5.8905);
    float cosY = cos(vPosition.y * 5.8905);
    float cosZ = cos(vPosition.z * 5.8905);
    oColor = (sinX * sinX + cosY * cosY + cosZ * cosZ) * (1.0f / 3.0f);
}
