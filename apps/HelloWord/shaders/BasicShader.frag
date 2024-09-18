#version 330 core
in vec3 ourColor;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float factor;

void main()
{
    FragColor = mix(texture(texture1, texCoord), texture(texture2, vec2(-texCoord.x, texCoord.y)), factor);
}
