#version 330 core

layout (location = 0) in vec2 aPos;

const vec3 planePos[6] = vec3[] (
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(-1, 1, 0),
    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(1, -1, 0)
);

uniform mat4 projection;
uniform mat4 view;

out vec3 nearPoint;
out vec3 farPoint;
out mat4 fragProjection;
out mat4 fragView;

vec3 unprojectPoint(float x, float y, float z, mat4 inversedView, mat4 inversedProjection) {
    vec4 unprojectedPoint = inversedView * inversedProjection * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 p = planePos[gl_VertexID];
    mat4 inversedProjection = inverse(projection);
    mat4 inversedView = inverse(view);
    nearPoint = unprojectPoint(p.x, p.y, 0.0, inversedView, inversedProjection);
    farPoint = unprojectPoint(p.x, p.y, 1.0, inversedView, inversedProjection);

    gl_Position = vec4(p, 1.0);
}