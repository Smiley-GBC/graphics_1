#version 330 core
out vec4 FragColor;
in vec3 color;

uniform vec3 u_color;
uniform float u_time;

void main()
{
    // Vector-Vector colour multiplication is just x * x, y * y, and z * z (or r * r, g * g, b * b)
    // For example, if our input colour is white and we multiply it by red, we get red: (1.0 * 1.0 = 1.0, 1.0 * 0.0 = 0.0, 1.0 * 0.0 = 0.0)
    //FragColor = vec4(color * u_color * u_time, 1.0f);
    FragColor = vec4(color * u_color, 1.0f);
}
