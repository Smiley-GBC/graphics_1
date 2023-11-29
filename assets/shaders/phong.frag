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

uniform float u_x;
uniform float u_x2;
uniform float u_x3;
uniform float u_ease;

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

    // t is a value ranging from 0 to 1
    float t = clamp(1.0 - length(u_light.position - position) / u_light.radius, 0.0, 1.0);

    // We can remap t using functions to make attenuation more interesting!
    float x = t;
    float x2 = pow(2.0, t) - 1.0;
    float x3 = pow(3.0, t) - 1.0;
    float ease = -(cos(t * 3.1415) - 1) * 0.5;  // same as smoothstep

    x *= u_x;
    x2 *= u_x2;
    x3 *= u_x3;
    ease *= u_ease;
    float attenuation = x + x2 + x3 + ease;

    //float attenuation = smoothstep(u_light.radius, 0.0, length(u_light.position - position));
    
    vec3 lighting = vec3(0.0);
    lighting += u_material.ambient * u_light.ambient;
    lighting += u_material.diffuse * u_light.diffuse * diffuse;
    lighting += u_material.specular * u_light.specular * specular;
    lighting *= attenuation;

    FragColor = vec4(lighting, 1.0);
}
