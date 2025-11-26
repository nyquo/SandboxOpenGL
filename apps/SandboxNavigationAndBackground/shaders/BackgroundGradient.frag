#version 330 core

in vec2 pos;

out vec4 FragColor;

uniform vec3 topLeftColor;
uniform vec3 topRightColor;
uniform vec3 bottomLeftColor;
uniform vec3 bottomRightColor;

void main() {
    // clamp pos betwen 0 and 1
    vec2 clampedPos = pos + vec2(1.0, 1.0);
    clampedPos *= 0.5;

    vec3 top = mix(topLeftColor, topRightColor, clampedPos.x);
    vec3 bottom = mix(bottomLeftColor, bottomRightColor, clampedPos.x);
    vec3 color = mix(bottom, top, clampedPos.y);
    FragColor = vec4(color, 1.0);
}