#version 460 core

out vec4 FragColor;

uniform vec3 u_color;

in vec3 normal;
in vec3 color;

void main()
{
    vec4 c = vec4(normalize(color), 1.0);
    vec3 n = normalize(normal);

    FragColor = c;
}
