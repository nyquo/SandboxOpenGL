#version 330 core
out vec4 FragColor;

uniform vec4 cpuColor;

void main()
{
   FragColor = cpuColor;
}
