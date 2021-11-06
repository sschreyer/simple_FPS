#version 330 core

// TODO: make this into a uniform?
#define NUM_LIGHTS 2

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 camPos;
uniform Material material;
uniform PointLight pointLights[NUM_LIGHTS];

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    // ambient light
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // diffuse

    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));


    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;

    return result;
}

void main() {

    vec3 result = vec3(0,0,0);
    vec3 n = normalize(Normal);
    vec3 viewDir = normalize(camPos - FragPos);
    for (int i = 0; i < NUM_LIGHTS; i++) {
        result += calcPointLight(pointLights[i], n, viewDir);
    }


    FragColor = vec4(result, 1.0);
}


