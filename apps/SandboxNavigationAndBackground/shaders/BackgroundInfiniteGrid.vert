#version 330 core

layout (location = 0) in vec2 aPos;

uniform mat4 inversedProjection;
uniform mat4 inversedView;

out vec2 pos;
out vec3 nearPoint;
out vec3 farPoint;

vec3 unprojectPoint(float x, float y, float z, mat4 inversedView, mat4 inversedProjection) {
    vec4 unprojectedPoint = inversedView * inversedProjection * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    pos = aPos;

    vec3 p = vec3(aPos, 0.0);
    nearPoint = unprojectPoint(p.x, p.y, 0.0, inversedView, inversedProjection);
    farPoint = unprojectPoint(p.x, p.y, 1.0, inversedView, inversedProjection);

    gl_Position = vec4(p, 1.0);
}