#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // DIRECTIONAL LIGHT
    // vec3 direction;

    // POSITION LIGHT
    // vec3 position;
    // float constant;
    // float linear;
    // float quadratic;

    // SPOT LIGHT
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

/**
  Note for later:
  vec4 lightVector could  repalce position or direction. if w == 0 it is a directional  light. if w == 1 it is position light

  */

void main()
{
    vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(-light.direction);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // DIRECTIONAL LIGHT
    // vec3 result = (ambient + diffuse + specular);

    // POINT LIGHT
    // float distance = length(light.position - FragPos);
    // float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // vec3 result = attenuation * (ambient + diffuse + specular);

    // SPOT LIGHT
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    vec3 result = ambient + intensity * (diffuse + specular);

    FragColor = vec4(result, 1.0);
}
