#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;
out vec3 fCenter;
out float fSize;

uniform mat4 view;
uniform mat4 projection;

vec4 applyProjectionView(vec4 position) {
    return projection * view * position;
}

void buildSquare(vec4 position) {
    fColor = gs_in[0].color;
    fCenter = vec3(0.0, 0.0, 0.0);
    fSize = 0.4;
    
    gl_Position = applyProjectionView(position + vec4(-0.2, -0.2, 0.0, 0.0));
    EmitVertex();
    gl_Position = applyProjectionView(position + vec4(0.2, -0.2, 0.0, 0.0));
    EmitVertex();
    gl_Position = applyProjectionView(position + vec4(-0.2, 0.2, 0.0, 0.0));
    EmitVertex();
    gl_Position = applyProjectionView(position + vec4(0.2, 0.2, 0.0, 0.0));
    EmitVertex();
    EndPrimitive();
}

void main() {
    buildSquare(gl_in[0].gl_Position);
}