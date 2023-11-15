#version 460 core

out vec4 FragColor;

uniform vec3 u_object_color;
uniform vec3 u_light_color;
uniform float u_ambient_strength;

in vec3 normal;

void main()
{
    vec3 lighting = vec3(0.0);
    vec3 ambient = u_light_color * u_ambient_strength; 
    lighting += ambient;

    vec3 finalColor = u_object_color * lighting;
    FragColor = vec4(finalColor, 1.0);
}
