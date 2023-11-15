#version 460 core

out vec4 FragColor;

uniform vec3 u_object_color;
uniform vec3 u_light_position;
uniform vec3 u_light_color;
uniform float u_ambient_strength;

in vec3 position;
in vec3 normal;

void main()
{
    vec3 lighting = vec3(0.0);
    vec3 ambient = u_light_color * u_ambient_strength; 
    lighting += ambient;

    // Vector FROM fragment TO light (because colour is based on what light reflects off of the surface)
    vec3 toLight = normalize(u_light_position - position);
    vec3 N = normalize(normal);
    float diffuseStrength = max(dot(N, toLight), 0.0f);
    vec3 diffuse = u_light_color * diffuseStrength;
    lighting += diffuse;

    vec3 finalColor = u_object_color * (normal * 0.5 + 0.5) * lighting;
    FragColor = vec4(finalColor, 1.0);
}
