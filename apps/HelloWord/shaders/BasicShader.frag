#version 330 core
in vec3 ourColor;
in vec3 vertexPosition;

out vec4 FragColor;

void main()
{
    FragColor = vec4(abs(vertexPosition.x), abs(vertexPosition.y), vertexPosition.z, 1.0);
   // FragColor = vec4(ourColor, 1.0);
}
