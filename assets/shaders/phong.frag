#version 330 core

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float radius;
};

uniform Material u_material;
uniform Light u_light;
uniform vec3 u_eye;

in vec3 position;   // world-space
in vec3 normal;     // world-space
in vec2 uv;

void main()
{
    vec3 N = normalize(normal);
    vec3 L = normalize(u_light.position - position);
    vec3 V = normalize(u_eye - position);
    vec3 R = reflect(-L, N);

    Material material;
    material.ambient = u_material.ambient;
    material.diffuse = u_material.diffuse;
    material.specular = u_material.specular;
    material.shininess = u_material.shininess;

    float diffuse = max(0.0, dot(N, L));
    float specular = pow(max(dot(V, R), 0.0), material.shininess);
    float attenuation = smoothstep(u_light.radius, 0.0, length(u_light.position - position));
    
    vec3 lighting = vec3(0.0);
    lighting += u_material.ambient * u_light.ambient;
    lighting += u_material.diffuse * u_light.diffuse * diffuse;
    lighting += u_material.specular * u_light.specular * specular;
    lighting *= attenuation;

    FragColor = vec4(lighting, 1.0);
}
