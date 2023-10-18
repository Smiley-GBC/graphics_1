#version 330 core
out vec4 FragColor;
in vec3 color;
in vec3 normal;

uniform vec3 u_color;

void main()
{
    FragColor = vec4(u_color, 1.0);
}
