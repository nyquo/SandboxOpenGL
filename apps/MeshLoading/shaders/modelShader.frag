#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NB_POINT_LIGHTS 4

//in vec3 Normal;
//in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
//uniform vec3 viewPos;
//uniform Material material;
//uniform DirLight dirLight;
//uniform PointLight pointLights[NB_POINT_LIGHTS];
//uniform SpotLight spotLight;

// vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
// vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
// vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    //vec3 norm = normalize(Normal);
    //vec3 viewDir = normalize(viewPos - FragPos);

    // Directional light
    //vec3 result = calcDirLight(dirLight, norm, viewDir);

    // Point lights
    //for(int i = 0; i < NB_POINT_LIGHTS; i++) {
    //    result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
    //}

    //result += calcSpotLight(spotLight, norm, FragPos, viewDir);

    //FragColor = vec4(result, 1.0);
    FragColor = texture(texture_diffuse1, TexCoords);
}

// vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
//     vec3 lightDir = normalize(-light.direction);

//     // Ambient Light
//     vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

//     // Diffuse light
//     float diff = max(dot(normal, lightDir), 0.0);
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

//     // Specular light
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

//     return (ambient + diffuse + specular);
// }

// vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
//     vec3 lightDir = normalize(light.position - FragPos);

//     // Ambient Light
//     vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

//     // Diffuse light
//     float diff = max(dot(normal, lightDir), 0.0);
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

//     // Specular light
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

//     float distance = length(light.position - fragPos);
//     float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

//     return attenuation * (ambient + diffuse + specular);
// }


// vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
//     vec3 lightDir = normalize(light.position - FragPos);

//     // Ambient Light
//     vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

//     // Diffuse light
//     float diff = max(dot(normal, lightDir), 0.0);
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

//     // Specular light
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

//     float distance = length(light.position - fragPos);
//     float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

//     float theta = dot(lightDir, normalize(-light.direction));
//     float epsilon = light.cutOff - light.outerCutOff;
//     float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

//     return (attenuation * intensity) * (ambient + diffuse + specular);
// }