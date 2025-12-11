#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;
out vec3 fCenter;
out float fSize;
out vec3 fFragPos;

uniform mat4 projectionView;

vec4 applyProjectionView(vec4 position) {
    return projectionView * position;
}

void emitVertex(vec4 center, vec4 position) { // Position in model space
    fFragPos = position.xyz;
    gl_Position = applyProjectionView(center + position);
    EmitVertex();
}

void buildSquare(vec4 position) {
    fColor = gs_in[0].color;
    fCenter = vec3(0.0, 0.0, 0.0);
    fSize = 0.4;

    emitVertex(position, vec4(-0.2, -0.2, -0.2, 0.0));
    emitVertex(position, vec4(0.2, -0.2, -0.2, 0.0));
    emitVertex(position, vec4(-0.2, 0.2, -0.2, 0.0));
    emitVertex(position, vec4(0.2, 0.2, -0.2, 0.0));
    EndPrimitive();

    emitVertex(position, vec4(-0.2, -0.2, 0.2, 0.0));
    emitVertex(position, vec4(0.2, -0.2, 0.2, 0.0));
    emitVertex(position, vec4(-0.2, 0.2, 0.2, 0.0));
    emitVertex(position, vec4(0.2, 0.2, 0.2, 0.0));
    EndPrimitive();

    emitVertex(position, vec4(0.2, -0.2, 0.2, 0.0));
    emitVertex(position, vec4(0.2, -0.2, -0.2, 0.0));
    emitVertex(position, vec4(0.2, 0.2, 0.2, 0.0));
    emitVertex(position, vec4(0.2, 0.2, -0.2, 0.0));
    EndPrimitive();

    emitVertex(position, vec4(-0.2, -0.2, 0.2, 0.0));
    emitVertex(position, vec4(-0.2, -0.2, -0.2, 0.0));
    emitVertex(position, vec4(-0.2, 0.2, 0.2, 0.0));
    emitVertex(position, vec4(-0.2, 0.2, -0.2, 0.0));
    EndPrimitive();

    emitVertex(position, vec4(-0.2, 0.2, 0.2, 0.0));
    emitVertex(position, vec4(0.2, 0.2, 0.2, 0.0));
    emitVertex(position, vec4(-0.2, 0.2, -0.2, 0.0));
    emitVertex(position, vec4(0.2, 0.2, -0.2, 0.0));
    EndPrimitive();

    emitVertex(position, vec4(-0.2, -0.2, 0.2, 0.0));
    emitVertex(position, vec4(0.2, -0.2, 0.2, 0.0));
    emitVertex(position, vec4(-0.2, -0.2, -0.2, 0.0));
    emitVertex(position, vec4(0.2, -0.2, -0.2, 0.0));
    EndPrimitive();
}

void main() {
    buildSquare(gl_in[0].gl_Position);
}