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

uniform sampler2D u_diffuse;
uniform sampler2D u_specular;
uniform float u_shininess;

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
    material.ambient = texture(u_diffuse, uv).rgb;
    material.diffuse = texture(u_diffuse, uv).rgb;
    material.specular = vec3(1.0, 1.0, 1.0);
    material.shininess = u_shininess;

    float diffuse = max(0.0, dot(N, L));
    float specular = pow(max(dot(V, R), 0.0), material.shininess);
    float attenuation = smoothstep(u_light.radius, 0.0, length(u_light.position - position));
    
    vec3 lighting = vec3(0.0);
    lighting += material.ambient * u_light.ambient;
    lighting += material.diffuse * u_light.diffuse * diffuse;
    lighting += material.specular * u_light.specular * specular * texture(u_specular, uv).r;
    lighting *= attenuation;
    
    FragColor = vec4(lighting, 1.0);
}
