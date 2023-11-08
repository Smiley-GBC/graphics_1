#version 460 core

out vec4 FragColor;

uniform vec3 u_color;
uniform sampler2D u_tex_slot_a;
uniform sampler2D u_tex_slot_b;
uniform float u_t;

in vec3 normal;
in vec3 color;
in vec2 uv;

void main()
{
    vec3 texColorA = texture(u_tex_slot_a, uv).rgb;
    vec3 texColorB = texture(u_tex_slot_b, uv).rgb;
    FragColor = vec4(mix(texColorA, texColorB, u_t) * u_color, 1.0);
}
