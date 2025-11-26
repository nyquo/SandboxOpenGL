#version 330 core

in vec2 pos;

out vec4 FragColor;

uniform vec3 innerColor;
uniform vec3 outerColor;
uniform float radius;
uniform float softness;

void main() {
    float dist = distance(pos, vec2(0.0, 0.0));

    float diff = clamp(1.0 - softness, 0.0, 1.0);
    float clampedRadius = clamp(radius, 0.0, 1.0);
    float smoothMin = diff * clampedRadius;
    float smoothMax = smoothMin + softness;
    float smoothDist = smoothstep(smoothMin, smoothMax, dist);
    vec3 color = mix(innerColor, outerColor, smoothDist);
    FragColor = vec4(color, 1.0);
}