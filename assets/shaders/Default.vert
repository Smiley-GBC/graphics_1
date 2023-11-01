#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexure;

uniform mat4 u_mvp;
//uniform mat4 u_m;
//uniform mat4 u_v;
//uniform mat4 u_p;

out vec3 color;
out vec3 normal;

void main()
{
   vec4 position = vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
   normal = normalize(aNormal);
   color = normal * 0.5 + 0.5;
   gl_Position = u_mvp * position;
   //gl_Position = u_p * u_v * u_m * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
