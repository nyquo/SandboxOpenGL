#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out VS_OUT {
    vec2 texCoords;
} geom_in[];

out vec2 fTexCoords;

void main() {
    fTexCoords = geom_in[0].texCoords;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    fTexCoords = geom_in[1].texCoords;
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    fTexCoords = geom_in[2].texCoords;
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
}