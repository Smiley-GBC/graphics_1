#version 330 core

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float radius;
};

struct DirectionLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material u_material;
#define MAX_POINT_LIGHTS 5

uniform DirectionLight u_direction_light;
uniform PointLight u_point_lights[MAX_POINT_LIGHTS];
uniform vec3 u_eye;

in vec3 position;   // world-space
in vec3 normal;     // world-space
in vec2 uv;

vec3 phong(Material material, PointLight light)
{
    vec3 N = normalize(normal);
    vec3 L = normalize(light.position - position);
    vec3 V = normalize(u_eye - position);
    vec3 R = reflect(-L, N);

    float diffuse = max(0.0, dot(N, L));
    float specular = pow(max(dot(V, R), 0.0), material.shininess);
    float attenuation = smoothstep(light.radius, 0.0, length(light.position - position));
    
    vec3 lighting = vec3(0.0);
    lighting += material.ambient * light.ambient;
    lighting += material.diffuse * light.diffuse * diffuse;
    lighting += material.specular * light.specular * specular;
    lighting *= attenuation;

    return lighting;
}

vec3 phong(Material material, DirectionLight light)
{
    vec3 N = normalize(normal);
    vec3 L = normalize(light.direction);
    vec3 V = normalize(u_eye - position);
    vec3 R = reflect(-L, N);

    float diffuse = max(0.0, dot(N, L));
    float specular = pow(max(dot(V, R), 0.0), material.shininess);
    
    vec3 lighting = vec3(0.0);
    lighting += material.ambient * light.ambient;
    lighting += material.diffuse * light.diffuse * diffuse;
    lighting += material.specular * light.specular * specular;

    return lighting;
}

void main()
{
    vec3 lighting = vec3(0.0);
    lighting += phong(u_material, u_direction_light);
    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
        lighting += phong(u_material, u_point_lights[i]);

    FragColor = vec4(lighting, 1.0);
}
