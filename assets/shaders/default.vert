#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexure;

uniform mat4 u_mvp;
uniform mat4 u_m;
//uniform mat4 u_m;
//uniform mat4 u_v;
//uniform mat4 u_p;

out vec3 position;   // world-space
out vec3 normal;     // world-space
out vec3 color;
out vec2 uv;

void main()
{
   vec4 position4 = vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
   position = (u_m * position4).xyz;
   normal = mat3(u_m) * aNormal;
   //normal = normalize(aNormal);
   color = normal * 0.5 + 0.5;
   uv = aTexure;
   gl_Position = u_mvp * position4;
   //gl_Position = u_p * u_v * u_m * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
