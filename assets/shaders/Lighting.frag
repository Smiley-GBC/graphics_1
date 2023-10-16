#version 330 core
out vec4 FragColor;
in vec3 color;
in vec3 normal;

uniform vec3 u_color;

void main()
{
    vec3 n = normalize(normal);
    vec3 light = normalize(vec3(0.0, -1.0, 1.0));
    float strength = dot(n, light);

    FragColor = vec4(u_color * color * strength, 1.0);//vec4(color * u_color, 1.0f);
}
