#version 330 core
out vec4 FragColor;
in vec3 color;

uniform vec3 u_color;
uniform float u_time;

void main()
{
    FragColor = vec4(color * u_color * (sin(u_time) * 0.5 + 0.5), 1.0f);
}
