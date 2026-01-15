#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float gamma;

void main()
{
    // FragColor = texture(screenTexture, TexCoords);
    vec3 fragColor = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(pow(fragColor, vec3(1.0/gamma)), 1.0);
}