#version 330 core
out vec4 FragColor;

in vec3 fColor;
in vec3 fCenter;
in float fSize;
in vec3 fFragPos;

void main()
{
    
    float dist = distance(fFragPos, fCenter);
    float halfSizeSquared = pow(fSize/2, 2);
    float distToCorner = sqrt(halfSizeSquared + halfSizeSquared);
    vec3 dimmed = mix(fColor, fColor * 0.3, dist/distToCorner);
    FragColor = vec4(dimmed, 1.0);   
}  