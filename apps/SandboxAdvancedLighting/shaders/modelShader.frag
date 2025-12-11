#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
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

#define NB_POINT_LIGHTS_MAX 10

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NB_POINT_LIGHTS_MAX];
uniform int nbPointLights;
uniform bool blinn;

vec4 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec4 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec4 texColor = texture(material.texture_diffuse1, TexCoords);
    if(texColor.a < 0.1) {
        discard;
    }

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Directional light
    vec4 result = calcDirLight(dirLight, norm, viewDir);

    // Point lights
    for(int i = 0; i < nbPointLights; i++) {
       result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    FragColor = result;
}

vec4 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    // Ambient Light
    vec4 ambient = vec4(light.ambient, 1) * vec4(texture(material.texture_diffuse1, TexCoords));

    // Diffuse light
    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = vec4(light.diffuse, 1) * diff * vec4(texture(material.texture_diffuse1, TexCoords));

    // Specular light
    float spec = 0.0;
    if(blinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 4 * material.shininess);
    } else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }
    vec4 specular = vec4(light.specular, 1) * spec;// * vec4(texture(material.texture_specular1, TexCoords));

    return (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - FragPos);

    // Ambient Light
    vec4 ambient = vec4(light.ambient, 1) * vec4(texture(material.texture_diffuse1, TexCoords));

    // Diffuse light
    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = vec4(light.diffuse, 1) * diff * vec4(texture(material.texture_diffuse1, TexCoords));

    // Specular light
    float spec = 0.0;
    if(blinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 4 * material.shininess);
    } else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }
    vec4 specular = vec4(light.specular, 1) * spec;// * vec4(texture(material.texture_specular1, TexCoords));

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return attenuation * (ambient + diffuse + specular);
}