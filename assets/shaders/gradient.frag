#version 460 core

out vec4 FragColor;

in vec2 uv;
uniform float u_time;

vec3 palette(float t, vec3 a, vec3 b, vec3 c, vec3 d)
{
    return a + b * cos(6.28318 * (c * t + d));
}

vec3 palette(float t, vec3 abcd[4])
{
    return abcd[0] + abcd[1] * cos(6.28318 * (abcd[2] * t + abcd[3]));
}

void main()
{
    // rainbow
    vec3 p0[4] = vec3[4](vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), vec3(0.0, 0.33, 0.67));

    // orange-brown-blue-white
    vec3 p1[4] = vec3[4](vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), vec3(0.3, 0.20, 0.20));

    // yellow-brown-blue-teal
    vec3 p2[4] = vec3[4](vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 0.5), vec3(0.8, 0.90, 0.30));

    // All kinda ugly xD
    //vec3 p2[4] = vec3[4](vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), vec3(0.0, 0.10, 0.20));
    //vec3 p3[4] = vec3[4](vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(2.0, 1.0, 0.0), vec3(0.5, 0.20, 0.25));
    //vec3 p4[4] = vec3[4](vec3(0.8, 0.5, 0.4), vec3(0.2, 0.4, 0.2), vec3(2.0, 1.0, 1.0), vec3(0.0, 0.25, 0.25));

    //float c = cos(u_time) * 0.5 + 0.5;
    //float s = cos(u_time) * 0.5 + 0.5;
    //FragColor = vec4(uv.x * c, uv.y, uv.x * s, 1.0);

    vec3 rainbow = palette(uv.x, p0);
    vec3 c0 = palette(uv.x, p1);
    vec3 c1 = palette(uv.x, p2);

    vec3 col = mix(c0, c1, uv.y);
    float t = cos(u_time) * 0.5 + 0.5;
    FragColor = vec4(mix(col, rainbow, t), 1.0);
}