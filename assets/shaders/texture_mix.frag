#version 330 core

out vec4 FragColor;

uniform float u_rainbow;
uniform float u_time;
uniform vec3 u_color;
uniform sampler2D u_tex;
uniform sampler2D u_tex2;

in vec3 normal;
in vec3 color;
in vec2 uv;

void main()
{
    vec3 color = normalize(color);
    vec3 tex1 = texture(u_tex, uv).rgb;
    vec3 tex2 = texture(u_tex2, uv).rgb;
    vec3 rgb = mix(tex1, tex2, cos(u_time) * 0.5 + 0.5);
    FragColor = vec4(mix(rgb, color, u_rainbow), 1.0);
}
