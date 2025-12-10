#version 330 core
out vec4 FragColor;

in vec3 fColor;
in vec3 fCenter;
in float fSize;

void main()
{
    float dist = distance(gl_Position, fCenter);
    vec3 dimmed = mix(vec3(0.0), vec3(1.0), 0.5);
    //vec3 dimmed = mix(fColor, 0.5 * fColor, 0.8);//dist/fSize);
    FragColor = vec4(dimmed, 1.0);   
}  