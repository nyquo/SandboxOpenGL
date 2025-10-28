#version 330 core

in vec2 pos;

out vec4 FragColor;

void main() {
    vec3 backgroundColor = vec3(0.1, 0.1, 0.1);

    FragColor = vec4(backgroundColor, 1.0);
}