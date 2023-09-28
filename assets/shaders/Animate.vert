#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 color;
uniform float u_time;

vec2 rotate(vec2 v, float angle)
{
   float c = cos(angle);
   float s = sin(angle);
   vec2 r = vec2(v.x * c - v.y * s, v.x * s + v.y * c);
   return r;
}

void main()
{
   color = aCol;
   float n = sin(u_time) * 0.5 + 0.5;
   vec2 s = vec2(n, n);
   vec2 p = aPos.xy * s;
   vec2 r = rotate(p, u_time * radians(100.0));
   vec2 t = vec2(cos(u_time), sin(u_time));
   gl_Position = vec4(r + t, aPos.z, 1.0);
}
