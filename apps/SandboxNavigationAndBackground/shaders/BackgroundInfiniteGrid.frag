#version 330 core

// inspiration: https://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/
in vec3 nearPoint;
in vec3 farPoint;

out vec4 FragColor;

// Put values in px later?
const float cellSize = 0.1; // in ndc
const float thickness = 1.0;
const vec3 backgroundColor = vec3(0.1, 0.1, 0.1);
const vec3 linesColor = vec3(0.5, 0.5, 0.5);
    

// Returns 1 if on lines, 0 overwise
vec4 grid(vec3 fragPos3D, float scale) {
    vec2 coord = fragPos3D.xz * scale; // use the scale variable to set the distance between the lines
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(linesColor, 1.0 - min(line, 1.0));
    // z axis
    if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx) {
        color.z = 1.0;
        color.a = 1.0;
    }
    // x axis
    if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz) {
        color.x = 1.0;
        color.a = 1.0;
    }
    return color;
}

void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);

    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    vec4 gridColor = grid(fragPos3D, 2);
    vec4 color = mix(vec4(backgroundColor, 1.0), gridColor, gridColor.a);

    FragColor =  color * float(t > 0);
}