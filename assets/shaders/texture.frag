#version 460 core

out vec4 FragColor;

uniform vec3 u_color;
uniform sampler2D u_tex_slot;

in vec3 normal;
in vec3 color;
in vec2 uv;

void main()
{
    vec3 texColor = texture(u_tex_slot, uv).rgb;
    FragColor = vec4(texColor * u_color, 1.0);
}
