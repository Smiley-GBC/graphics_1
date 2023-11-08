#version 460 core

out vec4 FragColor;

uniform vec3 u_color;

in vec3 normal;
in vec3 color;

void main()
{
    FragColor = vec4(u_color, 1.0);
}
