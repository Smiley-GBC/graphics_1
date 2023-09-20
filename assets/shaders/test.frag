#version 330 core
out vec4 FragColor;
in vec3 color;
void main()
{
    vec3 inColor = color;

    // Vector-Vector colour multiplication is just x * x, y * y, and z * z (or r * r, g * g, b * b)
    // For example, if our input colour is white and we multiply it by red, we get red: (1.0 * 1.0 = 1.0, 1.0 * 0.0 = 0.0, 1.0 * 0.0 = 0.0)
    vec3 newColor = vec3(1.0, 1.0, 1.0) * vec3(1.0, 0.0, 0.0);
   //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
   //FragColor = vec4(color, 1.0f);
   FragColor = vec4(newColor, 1.0f);
}
