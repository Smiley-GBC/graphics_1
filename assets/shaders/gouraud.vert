#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexure;

uniform mat4 u_mvp;
uniform mat4 u_model;
uniform mat3 u_normal;

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

out vec3 color;

void main()
{
    vec3 position = (u_model * vec4(aPosition, 1.0)).xyz;
    vec3 normal = u_normal * aNormal;
    vec2 uv = aTexure;

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
    color = lighting;

    gl_Position = u_mvp * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
}
